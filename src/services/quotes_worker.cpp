#include "quotes_worker.h"
#include <chrono>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <thread>
#include "global_config.h"
#include "common.h"

QuotesWorker::QuotesWorker() : postgres_connection(DB_CONNECTION_INFO) {
}

QuotesWorker::~QuotesWorker() {
}

void QuotesWorker::parse_quotes() {
  std::random_device              rd;
  std::mt19937                    gen(rd());
  std::uniform_int_distribution<> wait_seconds_distributor(5, 10);
  unsigned int                    offset = 0;
  unsigned int                    step   = 100;

  while (true) {
    int is_fetching_over = fetch_quotes(offset, step);

    if (is_fetching_over) {
      fmt::print("[QuotesWorker]: The quotes are over, parsing completed\n");
      break;
    }

    offset += step;

    int wait_seconds = wait_seconds_distributor(gen);
    fmt::print("[QuotesWorker]: fetched data, waiting {} seconds...\n", wait_seconds);
    std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));
  }
}

int QuotesWorker::fetch_quotes(unsigned int offset, unsigned int step) {
  std::random_device              rd;
  std::mt19937                    gen(rd());
  std::uniform_int_distribution<> header_index_distributor(0, 9);
  nlohmann::json                  quotes_headers_json;
  nlohmann::json                  body = {
      {"filter", nlohmann::json::array()},
      {"options", {{"lang", "en"}}},
      {"markets", {"crypto"}},
      {"symbols", {{"query", {{"types", nlohmann::json::array()}}}, {"tickers", nlohmann::json::array()}}},
      {"columns",
                        {"base_currency", "base_currency_desc", "type", "exchange", "crypto_total_rank", "close", "currency",
                         "24h_close_change|5", "market_cap_calc", "24h_vol_cmc", "circulating_supply"}},
      {"sort", {{"sortBy", "crypto_total_rank"}, {"sortOrder", "asc"}}},
      {"price_conversion", {{"to_symbol", false}}},
      {"range", {offset, offset + step}},
      {"ignore_unknown_fields", false},
      {"preset", "coin_market_cap_rank"}};

  int header_index = header_index_distributor(gen);

  std::ifstream file("../resources/quotes_headers.json");
  if (!file.is_open()) {
    fmt::print("[QuotesWorker]: Failed to open quotes_headers.json\n");
    return 1;
  }

  file >> quotes_headers_json;

  std::vector<std::string> headers       = common::build_headers_from_json(quotes_headers_json["headers"][header_index]);
  nlohmann::json           response_json = http_client.request_json(QUOTES_URL, "POST", body.dump(), headers);

  if (response_json.is_discarded() || response_json.is_null()) {
    fmt::print("[QuotesWorker]: Failed to parse or empty JSON response\n");
    return 1;
  }

  if (!response_json.contains("data") || !response_json["data"].is_array()) {
    fmt::print("[QuotesWorker]: Response JSON missing 'data' array\n");
    return 1;
  }

  for (const auto& symbol : response_json["data"]) {
    if (symbol.contains("d")) {
      Quote quote = Quote::from_json(symbol["d"]);

      postgres_connection.upsert_quote(quote);
    }
  }

  fmt::print("[QuotesWorker]: Quotes updated successfully\n");
  return 0;
}