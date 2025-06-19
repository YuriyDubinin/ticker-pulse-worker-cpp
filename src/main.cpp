#include <fmt/core.h>
#include <libpq-fe.h>
#include "global_config.h"

int main() {
    fmt::print("\n[TICKER_PULSE_WORKER]: started, version: {}", VERSION);

    PGconn *connection = PQconnectdb(CONNECTION_INFO);
    if (PQstatus(connection) != CONNECTION_OK) {
        fmt::print(stderr, "[ERROR] [TICKER_PULSE_WORKER]: {}\n", PQerrorMessage(connection));
        PQfinish(connection);
        return 1;
    }

    fmt::print("\n[TICKER_PULSE_WORKER]: Connected to PostgreSQL successfully!\n");

    const char *query =
        "SELECT id, ticker, description, type, exchange, total_rank, currency "
        "FROM quotes ORDER BY total_rank ASC LIMIT 10;";
    PGresult *result = PQexec(connection, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        fmt::print(stderr, "[ERROR] [TICKER_PULSE_WORKER]: {}\n", PQerrorMessage(connection));
        PQclear(result);
        PQfinish(connection);
        return 1;
    }

    int rows = PQntuples(result);
    for (int i = 0; i < rows; i++) {
        std::string id          = PQgetvalue(result, i, 0);
        std::string ticker      = PQgetvalue(result, i, 1);
        std::string description = PQgetvalue(result, i, 2);
        std::string type        = PQgetvalue(result, i, 3);
        std::string exchange    = PQgetvalue(result, i, 4);
        std::string total_rank  = PQgetvalue(result, i, 5);
        std::string currency    = PQgetvalue(result, i, 6);

        fmt::print("ticker: {}, description: {}\n", ticker, description);
    }

    PQclear(result);
    PQfinish(connection);

    return 0;
}