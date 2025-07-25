#include "postgres_connection.h"
#include <fmt/core.h>

PostgresConnection::PostgresConnection(const std::string& connection_info) {
  conn = PQconnectdb(connection_info.c_str());
  if (PQstatus(conn) != CONNECTION_OK) {
    fmt::print(stderr, "[POSTGRESS_CONNECTION]: Failed to connect to PostgreSQL: {}\n", PQerrorMessage(conn));
    PQfinish(conn);
    conn = nullptr;
  }
}

PostgresConnection::~PostgresConnection() {
  close();
}

bool PostgresConnection::is_connected() const {
  return conn && PQstatus(conn) == CONNECTION_OK;
}

PGresult* PostgresConnection::execute_query(const std::string& query) {
  if (!is_connected())
    return nullptr;
  PGresult* result = PQexec(conn, query.c_str());

  if (PQresultStatus(result) != PGRES_TUPLES_OK && PQresultStatus(result) != PGRES_COMMAND_OK) {
    fmt::print(stderr, "[POSTGRESS_CONNECTION]: Query failed: {}\n", PQerrorMessage(conn));
    PQclear(result);
    return nullptr;
  }

  return result;
}

void PostgresConnection::clear_result(PGresult* result) {
  if (result) {
    PQclear(result);
  }
}

void PostgresConnection::close() {
  if (conn) {
    PQfinish(conn);
    conn = nullptr;
  }
}

PGconn* PostgresConnection::raw() {
  return conn;
}

void PostgresConnection::upsert_quote(const Quote& quote) {
  const char* query = R"SQL(
        INSERT INTO quotes (
            ticker, description, type, exchange, total_rank,
            last_close_price, currency, close_change_24h,
            market_cap_calc, vol_cmc_24h, circulating_supply
        ) VALUES (
            $1, $2, $3, $4, $5,
            $6, $7, $8,
            $9, $10, $11
        )
        ON CONFLICT (ticker) DO UPDATE SET
            description = EXCLUDED.description,
            type = EXCLUDED.type,
            exchange = EXCLUDED.exchange,
            total_rank = EXCLUDED.total_rank,
            last_close_price = EXCLUDED.last_close_price,
            currency = EXCLUDED.currency,
            close_change_24h = EXCLUDED.close_change_24h,
            market_cap_calc = EXCLUDED.market_cap_calc,
            vol_cmc_24h = EXCLUDED.vol_cmc_24h,
            circulating_supply = EXCLUDED.circulating_supply,
            updated_at = CURRENT_TIMESTAMP;
    )SQL";

  const char* paramValues[11];
  std::string rank_str       = std::to_string(quote.total_rank);
  std::string price_str      = std::to_string(quote.last_close_price);
  std::string change_24h_str = std::to_string(quote.close_change_24h);
  std::string market_cap_str = std::to_string(quote.market_cap_calc);
  std::string vol_cmc_str    = std::to_string(quote.vol_cmc_24h);
  std::string supply_str     = std::to_string(quote.circulating_supply);

  paramValues[0]  = quote.ticker.c_str();
  paramValues[1]  = quote.description.c_str();
  paramValues[2]  = quote.type.c_str();
  paramValues[3]  = quote.exchange.c_str();
  paramValues[4]  = rank_str.c_str();
  paramValues[5]  = price_str.c_str();
  paramValues[6]  = quote.currency.c_str();
  paramValues[7]  = change_24h_str.c_str();
  paramValues[8]  = market_cap_str.c_str();
  paramValues[9]  = vol_cmc_str.c_str();
  paramValues[10] = supply_str.c_str();

  PGresult* res = PQexecParams(conn, query, 11, nullptr, paramValues, nullptr, nullptr, 0);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fmt::print("[PostgresConnection]: Failed to upsert quote: {}\n", PQerrorMessage(conn));
  }

  PQclear(res);
}

void PostgresConnection::insert_news_if_not_exists(const News& news) {
  const char* query = R"SQL(
        INSERT INTO news (
            uid,
            title,
            source,
            source_logo_id,
            source_url,
            story_path,
            link
        ) VALUES (
            $1, $2, $3, $4, $5, $6, $7
        )
        ON CONFLICT (uid) DO NOTHING;
    )SQL";

  const char* paramValues[7];
  paramValues[0] = news.uid.c_str();
  paramValues[1] = news.title.c_str();
  paramValues[2] = news.source.c_str();
  paramValues[3] = news.source_logo_id.c_str();
  paramValues[4] = news.source_url.c_str();
  paramValues[5] = news.story_path.c_str();
  paramValues[6] = news.link.c_str();

  PGresult* res = PQexecParams(conn, query, 7, nullptr, paramValues, nullptr, nullptr, 0);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fmt::print("[PostgresConnection]: Failed to insert news: {}\n", PQerrorMessage(conn));
  }

  PQclear(res);
}

// Подготовить запрос
bool PostgresConnection::prepare_statement(const std::string& statement_name, const std::string& query) {
  if (!is_connected())
    return false;
  PGresult* res = PQprepare(conn, statement_name.c_str(), query.c_str(), 0, nullptr);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    fmt::print(stderr, "[POSTGRESS_CONNECTION]: Prepare failed: {}\n", PQerrorMessage(conn));
    PQclear(res);
    return false;
  }
  PQclear(res);
  return true;
}

// Выполнить подготовленный запрос
PGresult* PostgresConnection::execute_prepared_statement(const std::string& statement_name,
                                                         int                params_count,
                                                         const char* const* param_values) {
  if (!is_connected())
    return nullptr;
  PGresult* res = PQexecPrepared(conn, statement_name.c_str(), params_count, param_values, nullptr, nullptr, 0);
  if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) {
    fmt::print(stderr, "[POSTGRESS_CONNECTION]: ExecPrepared failed: {}\n", PQerrorMessage(conn));
    PQclear(res);
    return nullptr;
  }
  return res;
}
