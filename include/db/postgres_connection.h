#ifndef POSTGRESS_CONNECTION
#define POSTGRESS_CONNECTION

#include <libpq-fe.h>
#include <string>
#include "news_model.h"
#include "quote_model.h"

class PostgresConnection {
  public:
    explicit PostgresConnection(const std::string& connection_info);
    ~PostgresConnection();

    PostgresConnection(const PostgresConnection&) = delete;
    PostgresConnection& operator=(const PostgresConnection&) = delete;

    bool      isConnected() const;
    PGresult* executeQuery(const std::string& query);
    void      clearResult(PGresult* result);
    void      close();

    void upsert_quote(const Quote& quote);
    void insert_news_if_not_exists(const News& news);

    PGconn* raw(); // Доступ к PGconn* при необходимости

  private:
    PGconn* conn;
};

#endif
