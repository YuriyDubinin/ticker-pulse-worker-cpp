#ifndef POSTGRESS_CONNECTION_H
#define POSTGRESS_CONNECTION_H

#include <libpq-fe.h>
#include <string>
#include "news_model.h"
#include "quote_model.h"

class PostgresConnection {
  public:
    explicit PostgresConnection(const std::string& connection_info);
    ~PostgresConnection();

    PostgresConnection(const PostgresConnection&)            = delete;
    PostgresConnection& operator=(const PostgresConnection&) = delete;

    bool      is_connected() const;
    PGresult* execute_query(const std::string& query);
    void      clear_result(PGresult* result);
    void      close();

    void upsert_quote(const Quote& quote);
    void insert_news_if_not_exists(const News& news);

    PGconn* raw(); // Доступ к PGconn* при необходимости

    bool prepare_statement(const std::string& statement_name, const std::string& query);
    PGresult*
    execute_prepared_statement(const std::string& statement_name, int params_count, const char* const* param_values);

  private:
    PGconn* conn;
};

#endif
