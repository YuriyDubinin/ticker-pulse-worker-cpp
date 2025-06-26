#include "common.h"

namespace common {
  std::vector<std::string> build_headers_from_json(const nlohmann::json& json_headers) {
    std::vector<std::string> headers;

    for (auto& [key, value] : json_headers.items()) {
      headers.push_back(key + ": " + value.get<std::string>());
    }

    return headers;
  }

  std::string extract_AST(const nlohmann::json& node) {
    std::string result;

    if (node.contains("type") && (node["type"] == "p" || node["type"] == "quote")) {
      if (node.contains("children")) {
        for (const auto& child : node["children"]) {
          if (child.is_string()) {
            result += child.get<std::string>();
          } else if (child.contains("children")) {
            for (const auto& sub : child["children"]) {
              if (sub.is_string()) {
                result += sub.get<std::string>();
              }
            }
          }
        }
        result += "\n\n";
      }
    }
    if (node.contains("children") && node["children"].is_array()) {
      for (const auto& child : node["children"]) {
        result += extract_AST(child);
      }
    }

    return result;
  }

  std::string trim_text_to_limit(const std::string& text, size_t limit = 1024) {
    if (text.length() <= limit)
      return text;
    return text.substr(0, limit - 3) + "...";
  }

  std::string safe_string_from_json_array(const nlohmann::json& j, int index) {
    return (j.size() > index && !j.at(index).is_null()) ? j.at(index).get<std::string>() : "";
  }

  int safe_int_from_json_array(const nlohmann::json& j, int index) {
    return (j.size() > index && j.at(index).is_number_integer()) ? j.at(index).get<int>() : 0;
  }

  double safe_double_from_json_array(const nlohmann::json& j, int index) {
    return (j.size() > index && j.at(index).is_number()) ? j.at(index).get<double>() : 0.0;
  }

  std::string safe_string_from_json_object(const nlohmann::json& j, const std::string& key) {
    return (j.contains(key) && !j.at(key).is_null() && j.at(key).is_string()) ? j.at(key).get<std::string>() : "";
  }

  int safe_int_from_json_object(const nlohmann::json& j, const std::string& key) {
    return (j.contains(key) && j.at(key).is_number_integer()) ? j.at(key).get<int>() : 0;
  }

  double safe_double_from_json_object(const nlohmann::json& j, const std::string& key) {
    return (j.contains(key) && j.at(key).is_number()) ? j.at(key).get<double>() : 0.0;
  }
} // namespace common
