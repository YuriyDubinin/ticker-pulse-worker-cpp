#include "news_model.h"

News News::from_json(const nlohmann::json& json) {
  News news;

  if (json.contains("id") && json["id"].is_string()) {
    news.uid = json["id"];
  }

  if (json.contains("title") && json["title"].is_string()) {
    news.title = json["title"];
  }

  return news;
}
