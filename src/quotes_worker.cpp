#include "quotes_worker.h"
#include <fmt/core.h>
#include <iostream>
#include "global_config.h"
#include "http_client.h"

QuotesWorker::QuotesWorker() {
}

QuotesWorker::~QuotesWorker() {
}

void QuotesWorker::fetch_quotes() {
    HTTPClient http_client;

    nlohmann::json json_result = http_client.fetch_json(QUOTES_URL);

    if (!json_result.empty()) {
        fmt::print("[QUOTES_WORKER]: Quotes fetched:\n{}\n", json_result.dump(4));
    } else {
        fmt::print("[QUOTES_WORKER]: Failed to fetch quotes\n");
    }
}