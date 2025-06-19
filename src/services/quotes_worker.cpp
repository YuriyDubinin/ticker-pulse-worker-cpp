#include "quotes_worker.h"
#include <chrono>
#include <fmt/core.h>
#include <iostream>
#include <random>
#include <thread>
#include "global_config.h"

QuotesWorker::QuotesWorker() : postgres_connection(DB_CONNECTION_INFO) {
}

QuotesWorker::~QuotesWorker() {
}

void QuotesWorker::parse_quotes() {
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> distr(5, 10);

    while (true) {
        int is_fetched = fetch_quotes(100);

        if (!is_fetched) {
            fmt::print("[QuotesWorker]: NO_DATA\n");
            break;
        }

        int wait_seconds = distr(gen);
        fmt::print("[QuotesWorker]: fetched data, waiting {} seconds...\n", wait_seconds);
        std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
    }
}

int QuotesWorker::fetch_quotes(unsigned int offset) {
    nlohmann::json body = {
        {"filter", nlohmann::json::array()},
        {"options", {{"lang", "en"}}},
        {"markets", {"crypto"}},
        {"symbols", {{"query", {{"types", nlohmann::json::array()}}}, {"tickers", nlohmann::json::array()}}},
        {"columns",
         {"base_currency", "base_currency_desc", "type", "exchange", "crypto_total_rank", "close", "currency",
          "24h_close_change|5", "market_cap_calc", "24h_vol_cmc", "circulating_supply"}},
        {"sort", {{"sortBy", "crypto_total_rank"}, {"sortOrder", "asc"}}},
        {"price_conversion", {{"to_symbol", false}}},
        {"range", {0, 100}},
        {"ignore_unknown_fields", false},
        {"preset", "coin_market_cap_rank"}};

    std::vector<std::string> header        = {"Content-Type: application/json", "Accept: application/json"};
    nlohmann::json           response_json = http_client.request_json(QUOTES_URL, "POST", body.dump(), header);

    if (response_json.is_discarded() || response_json.is_null()) {
        fmt::print("[QuotesWorker]: Failed to parse or empty JSON response\n");
        return 0;
    }

    if (!response_json.contains("data") || !response_json["data"].is_array()) {
        fmt::print("[QuotesWorker]: Response JSON missing 'data' array\n");
        return 0;
    }

    for (const auto& symbol : response_json["data"]) {
        if (symbol.contains("d")) {
            Quote quote = Quote::from_json(symbol["d"]);
            // fmt::print("[QuotesWorker]: {}\n", symbol["d"].dump());
            postgres_connection.upsert_quote(quote);
        }
    }

    fmt::print("[QuotesWorker]: Quotes updated successfully\n");
    return 1;
}
