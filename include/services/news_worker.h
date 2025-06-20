#ifndef NEWS_WORKER
#define NEWS_WORKER

#include "http_client.h"
#include "postgres_connection.h"

class NewsWorker {
  public:
    explicit NewsWorker();
    ~NewsWorker();

    void parse_news();

  private:
    HTTPClient         http_client;
    PostgresConnection postgres_connection;

    int fetch_news(unsigned int offset, unsigned int step);
};

#endif