#ifndef QUOTE_MODEL_H
#define QUOTE_MODEL_H

#include <nlohmann/json.hpp>
#include <string>

struct Quote {
    std::string ticker;
    std::string description;
    std::string type;
    std::string exchange;
    int         total_rank;
    double      last_close_price;
    std::string currency;
    double      close_change_24h;
    double      market_cap_calc;
    double      vol_cmc_24h;
    double      circulating_supply;

    static Quote from_json(const nlohmann::json& json);
};

#endif
