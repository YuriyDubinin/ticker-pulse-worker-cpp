#include "http_client.h"
#include <curl/curl.h>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

HTTPClient::HTTPClient() {
  curl_global_init(CURL_GLOBAL_DEFAULT); // Инициализация глобального состояния CURL
}

HTTPClient::~HTTPClient() {
  curl_global_cleanup(); // Очистка глобального состояния CURL
}

size_t HTTPClient::write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
  // Функция вызывающаяся каждый раз, когда CURL получает данные
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

nlohmann::json HTTPClient::fetch_json(const std::string& url) {
  CURL*       curl;
  CURLcode    res;
  std::string readBuffer;

  // Инициализация CURL
  curl = curl_easy_init();

  if (curl) {
    // Установка URL и других параметров CURL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    // Выполнение запроса
    res = curl_easy_perform(curl);

    // Проверка на ошибки
    if (res != CURLE_OK) {
      fmt::print("[CRYPTO_FETCHER]: cURL error: {}\n", curl_easy_strerror(res));
    }

    // Очистка CURL
    curl_easy_cleanup(curl);
  }

  // Парс JSON
  if (!readBuffer.empty()) {
    try {
      return nlohmann::json::parse(readBuffer);
    } catch (const nlohmann::json::exception& e) {
      fmt::print("[CRYPTO_FETCHER]: JSON parsing error: {}\n", e.what());
    }
  }

  // Пустой JSON в случае ошибки
  return nlohmann::json();
}

nlohmann::json HTTPClient::request_json(const std::string&              url,
                                        const std::string&              method,
                                        const std::string&              body,
                                        const std::vector<std::string>& headers) {
  CURL*       curl = curl_easy_init();
  std::string readBuffer;

  if (!curl)
    return nlohmann::json();

  struct curl_slist* header_list = nullptr;
  for (const auto& h : headers) {
    header_list = curl_slist_append(header_list, h.c_str());
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

  if (!headers.empty()) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
  }

  if (method == "POST" || method == "PUT") {
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());
  }

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fmt::print("[HTTP_CLIENT]: cURL error: {}\n", curl_easy_strerror(res));
  }

  curl_easy_cleanup(curl);
  if (header_list)
    curl_slist_free_all(header_list);

  if (!readBuffer.empty()) {
    try {
      return nlohmann::json::parse(readBuffer);
    } catch (const nlohmann::json::exception& e) {
      fmt::print("[HTTP_CLIENT]: JSON parsing error: {}\n", e.what());
    }
  }

  return nlohmann::json();
}