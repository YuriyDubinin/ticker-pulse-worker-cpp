#include <fmt/core.h>
#include <libpq-fe.h>
#include "global_config.h"


int main() {
  fmt::print("\n[TICKER_PULSE_WORKER]: started, version: {}", VERSION);

  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  if (PQstatus(connection) != CONNECTION_OK) {
    fmt::print(stderr, "[ERROR] [TICKER_PULSE_WORKER]: {}\n", PQerrorMessage(connection));
    PQfinish(connection);
    return 1;
  }

  fmt::print("\n[TICKER_PULSE_WORKER]: Connected to PostgreSQL successfully!\n");

  const char* query = "SELECT id, ticker, description, type, exchange, total_rank, currency FROM quotes ORDER BY total_rank ASC LIMIT 10;";
  PGresult* res = PQexec(connection, query);

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
      fmt::print(stderr, "[ERROR] [TICKER_PULSE_WORKER]: {}\n", PQerrorMessage(connection));
      PQclear(res);
      PQfinish(connection);
      return 1;
  }

  int rows = PQntuples(res);
  for (int i = 0; i < rows; i++) {
      std::string id          = PQgetvalue(res, i, 0);
      std::string ticker      = PQgetvalue(res, i, 1);
      std::string description = PQgetvalue(res, i, 2);
      std::string type        = PQgetvalue(res, i, 3);
      std::string exchange    = PQgetvalue(res, i, 4);
      std::string total_rank  = PQgetvalue(res, i, 5);
      std::string currency    = PQgetvalue(res, i, 6);
  }

  PQclear(res);
  PQfinish(connection);

  return 0;
}