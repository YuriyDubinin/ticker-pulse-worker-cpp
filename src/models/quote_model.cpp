#include "quote_model.h"

Quote Quote::from_json(const nlohmann::json& json) {
  auto safe_string = [](const nlohmann::json& j, int index) -> std::string {
    return (j.size() > index && !j.at(index).is_null()) ? j.at(index).get<std::string>() : "";
  };

  auto safe_int = [](const nlohmann::json& j, int index) -> int {
    return (j.size() > index && j.at(index).is_number_integer()) ? j.at(index).get<int>() : 0;
  };

  auto safe_double = [](const nlohmann::json& j, int index) -> double {
    return (j.size() > index && j.at(index).is_number()) ? j.at(index).get<double>() : 0.0;
  };

  return Quote{
      safe_string(json, 0), // ticker
      safe_string(json, 1), // description
      safe_string(json, 2), // type
      safe_string(json, 3), // exchange
      safe_int(json, 4),    // total_rank
      safe_double(json, 5), // last_close_price
      safe_string(json, 6), // currency
      safe_double(json, 7), // close_change_24h
      safe_double(json, 8), // market_cap_calc
      safe_double(json, 9), // vol_cmc_24h
      safe_double(json, 10) // circulating_supply
  };
}
