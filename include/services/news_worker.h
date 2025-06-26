#ifndef NEWS_WORKER
#define NEWS_WORKER

#include "http_client.h"
#include "postgres_connection.h"

class NewsWorker {
  public:
    explicit NewsWorker();
    ~NewsWorker();

    NewsWorker(const NewsWorker&)            = delete;
    NewsWorker& operator=(const NewsWorker&) = delete;

    void fetch_news();

  private:
    HTTPClient         http_client;
    PostgresConnection postgres_connection;
};

#endif