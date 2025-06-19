#ifndef QUOTES_WORKER
#define QUOTES_WORKER

#include "http_client.h"
#include "postgres_connection.h"

class QuotesWorker {
  public:
    explicit QuotesWorker();
    ~QuotesWorker();

    void parse_quotes();

  private:
    HTTPClient         http_client;
    PostgresConnection postgres_connection;

    int fetch_quotes();
};

#endif