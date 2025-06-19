#ifndef POSTGRESS_CONNECTION
#define POSTGRESS_CONNECTION

#include <libpq-fe.h>
#include <string>
#include "quote_model.h"

class PostgresConnection {
  public:
    explicit PostgresConnection(const std::string& connection_info);
    ~PostgresConnection();

    bool      isConnected() const;
    PGresult* executeQuery(const std::string& query);
    void      clearResult(PGresult* result);
    void      close();

    void upsert_quote(const Quote& quote);

    PGconn* raw(); // Доступ к PGconn* при необходимости

  private:
    PGconn* conn;
};

#endif
