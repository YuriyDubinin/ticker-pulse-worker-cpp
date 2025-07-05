#include "news_worker.h"
#include <chrono>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include "common_utils.h"
#include "global_config.h"

NewsWorker::NewsWorker() : postgres_connection(DB_CONNECTION_INFO), categories{"etf", "crypto"} {
}

NewsWorker::~NewsWorker() {
}

void NewsWorker::fetch_news(const unsigned int category_index) {
  std::random_device              rd;
  std::mt19937                    gen(rd());
  std::uniform_int_distribution<> header_index_distributor(0, 9);
  nlohmann::json                  news_headers_json;
  nlohmann::json                  body              = {};
  std::string                     category_news_url = NEWS_URL + categories[category_index];

  int header_index = header_index_distributor(gen);

  std::ifstream file("../resources/news_headers.json");

  if (!file.is_open()) {
    fmt::print("[QuotesWorker]: Failed to open quotes_headers.json\n");
  }

  file >> news_headers_json;

  std::vector<std::string> headers = common_utils::build_headers_from_json(news_headers_json["headers"][header_index]);
  nlohmann::json           response_json = http_client.request_json(category_news_url, "GET", body.dump(), headers);

  std::reverse(response_json.begin(), response_json.end());

  for (const auto& headline : response_json["items"]) {
    News const news = News::from_json(headline);

    if (!news.link.empty()) {
      postgres_connection.insert_news_if_not_exists(news);
    }
  }

  // fmt::print("[NewsWorker]: News updated successfully\n");
}