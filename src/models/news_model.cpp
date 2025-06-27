#include "news_model.h"

#include "common_utils.h"

News News::from_json(const nlohmann::json& json) {
  News news;
  news.uid            = common_utils::safe_string_from_json_object(json, "id");
  news.title          = common_utils::safe_string_from_json_object(json, "title");
  news.source         = common_utils::safe_string_from_json_object(json, "source");
  news.source_logo_id = common_utils::safe_string_from_json_object(json, "sourceLogoId");
  news.source_url     = common_utils::safe_string_from_json_object(json, "sourceUrl");
  news.story_path     = common_utils::safe_string_from_json_object(json, "storyPath");
  news.link           = common_utils::safe_string_from_json_object(json, "link");

  return news;
}