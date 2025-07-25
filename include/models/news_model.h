#ifndef NEWS_MODEL_H
#define NEWS_MODEL_H

#include <nlohmann/json.hpp>
#include <string>

struct News {
    std::string uid;
    std::string title;
    // std::string content;
    std::string source;
    std::string source_logo_id;
    std::string source_url;
    std::string story_path;
    std::string link;

    static News from_json(const nlohmann::json& json);
};

#endif