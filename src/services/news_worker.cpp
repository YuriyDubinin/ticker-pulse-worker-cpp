#include "news_worker.h"
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include "global_config.h"

NewsWorker::NewsWorker() : postgres_connection(DB_CONNECTION_INFO) {
}

NewsWorker::~NewsWorker() {
}

void NewsWorker::fetch_news() {
  nlohmann::json body = {

  };
  std::vector<std::string> headers       = {"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)",
                                            "Accept: application/json", "Referer: https://www.tradingview.com/"};
  nlohmann::json           response_json = http_client.request_json(NEWS_URL, "GET", body.dump(), headers);

  for (const auto& headline : response_json.items()) {
    // fmt::print("\n{}\n", headline.value().dump());
    News const news = News::from_json(headline.value());

    if (!news.link.empty()) {
      postgres_connection.insert_news_if_not_exists(news);
    }
  }
}