#ifndef NEWS_WORKER_H
#define NEWS_WORKER_H

#include <string>
#include <vector>
#include "http_client.h"
#include "postgres_connection.h"

class NewsWorker {
  public:
    explicit NewsWorker();
    ~NewsWorker();

    NewsWorker(const NewsWorker&)            = delete;
    NewsWorker& operator=(const NewsWorker&) = delete;

    void fetch_news(const unsigned int category_index);

  private:
    HTTPClient               http_client;
    PostgresConnection       postgres_connection;
    std::vector<std::string> categories;
};

#endif