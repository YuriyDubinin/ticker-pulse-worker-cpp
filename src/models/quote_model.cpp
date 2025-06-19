#include "quote_model.h"

Quote Quote::from_json(const nlohmann::json& json) {
    return Quote{
        json.at(0).get<std::string>(), // ticker
        json.at(1).get<std::string>(), // description
        json.at(2).get<std::string>(), // type
        json.at(3).get<std::string>(), // exchange
        json.at(4).get<int>(),         // total_rank
        json.at(5).get<double>(),      // last_close_price
        json.at(6).get<std::string>(), // currency
        json.at(7).get<double>(),      // close_change_24h
        json.at(8).get<double>(),      // market_cap_calc
        json.at(9).get<double>(),      // vol_cmc_24h
        json.at(10).get<double>()      // circulating_supply
    };
}
