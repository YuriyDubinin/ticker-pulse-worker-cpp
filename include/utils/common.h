#ifndef COMMON
#define COMMON

#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace common {
  std::string extract_AST(const nlohmann::json& node);
  std::string trim_text_to_limit(const std::string& text, size_t limit);
  std::string safe_string(const nlohmann::json& j, int index);
  int         safe_int(const nlohmann::json& j, int index);
  double      safe_double(const nlohmann::json& j, int index);
} // namespace common

#endif