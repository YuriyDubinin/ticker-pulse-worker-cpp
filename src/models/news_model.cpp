#include "news_model.h"

#include "common.h"

News News::from_json(const nlohmann::json& json) {
  News news;
  news.uid            = common::safe_string_from_json_object(json, "id");
  news.title          = common::safe_string_from_json_object(json, "title");
  news.source         = common::safe_string_from_json_object(json, "source");
  news.source_logo_id = common::safe_string_from_json_object(json, "sourceLogoId");
  news.source_url     = common::safe_string_from_json_object(json, "sourceUrl");
  news.story_path     = common::safe_string_from_json_object(json, "storyPath");
  news.published      = common::safe_string_from_json_object(json, "published");
  news.link           = common::safe_string_from_json_object(json, "link");

  return news;
}