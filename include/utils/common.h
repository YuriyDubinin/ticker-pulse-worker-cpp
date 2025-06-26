#ifndef COMMON
#define COMMON

#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace common {
  std::vector<std::string> build_headers_from_json(const nlohmann::json& json_headers);
  std::string              extract_AST(const nlohmann::json& node);
  std::string              trim_text_to_limit(const std::string& text, size_t limit);
  std::string              safe_string_from_json_array(const nlohmann::json& j, int index);
  int                      safe_int_from_json_array(const nlohmann::json& j, int index);
  double                   safe_double_from_json_array(const nlohmann::json& j, int index);
  std::string              safe_string_from_json_object(const nlohmann::json& j, const std::string& key);
  int                      safe_int_from_json_object(const nlohmann::json& j, const std::string& key);
  double                   safe_double_from_json_object(const nlohmann::json& j, const std::string& key);
} // namespace common

#endif