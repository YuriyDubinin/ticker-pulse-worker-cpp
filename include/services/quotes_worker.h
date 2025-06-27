#ifndef QUOTES_WORKER_H
#define QUOTES_WORKER_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "http_client.h"
#include "postgres_connection.h"

class QuotesWorker {
  public:
    explicit QuotesWorker();
    ~QuotesWorker();

    QuotesWorker(const QuotesWorker&)            = delete;
    QuotesWorker& operator=(const QuotesWorker&) = delete;

    void parse_quotes();

  private:
    HTTPClient         http_client;
    PostgresConnection postgres_connection;

    int                      fetch_quotes(unsigned int offset, unsigned int step);
    std::vector<std::string> build_headers_from_json(const nlohmann::json& json_headers);
};

#endif