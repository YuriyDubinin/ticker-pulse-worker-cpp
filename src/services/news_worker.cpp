#include "news_worker.h"
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include "global_config.h"

NewsWorker::NewsWorker() : postgres_connection(DB_CONNECTION_INFO) {
}

NewsWorker::~NewsWorker() {
}

void NewsWorker::fetch_news() {
  nlohmann::json           body          = {};
  std::vector<std::string> headers       = {"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)",
                                            "Accept: application/json", "Referer: https://www.tradingview.com/"};
  nlohmann::json           response_json = http_client.request_json(NEWS_URL, "GET", body.dump(), headers);

  for (auto& [key, value] : response_json[0].items()) {
    fmt::print("{}: {}\n\n", key, value.dump());
  }
}