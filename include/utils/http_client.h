#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>

class HTTPClient {
  public:
    explicit HTTPClient();
    ~HTTPClient();

    nlohmann::json fetch_json(const std::string& url);
    nlohmann::json request_json(const std::string&              url,
                                const std::string&              method,
                                const std::string&              body    = "",
                                const std::vector<std::string>& headers = {});

  private:
    static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif