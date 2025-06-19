#include "quotes_worker.h"
#include <fmt/core.h>
#include <iostream>
#include "global_config.h"

QuotesWorker::QuotesWorker() : postgres_connection(DB_CONNECTION_INFO) {
}

QuotesWorker::~QuotesWorker() {
}

void QuotesWorker::parse_quotes() {
    fetch_quotes();
}

void QuotesWorker::fetch_quotes() {
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
        return;
    }

    if (!response_json.contains("data") || !response_json["data"].is_array()) {
        fmt::print("[QuotesWorker]: Response JSON missing 'data' array\n");
        return;
    }

    for (const auto& symbol : response_json["data"]) {
        if (symbol.contains("d")) {
            fmt::print("[QuotesWorker]: {}\n", symbol["d"].dump());
            // inserting;
            // upsert_quote(symbol["d"]);
        }
    }

    fmt::print("[QuotesWorker]: Quotes updated successfully\n");
}
