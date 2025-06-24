#include "common.h"

namespace common {
  // Рекурсивная функция для извлечения текста
  std::string extract_AST(const nlohmann::json& node) {
    std::string result;

    if (node.is_string()) {
      result += node.get<std::string>();
    }

    if (node.contains("children")) {
      for (const auto& child : node["children"]) {
        result += extract_AST(child);
      }
      // Добавление переноса строки, если родительский блок - абзац
      if (node.contains("type") && node["type"] == "p") {
        result += "\n";
      }
    }

    if (node.contains("type") && node["type"] == "symbol" && node.contains("params")) {
      if (node["params"].contains("text")) {
        result += node["params"]["text"].get<std::string>();
      }
    }

    return result;
  }
} // namespace common