#ifndef COMMON
#define COMMON

#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace common {
  std::string extract_AST(const nlohmann::json& node);
}

#endif