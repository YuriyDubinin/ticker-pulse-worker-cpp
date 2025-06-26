#include "quote_model.h"
#include "common.h"

Quote Quote::from_json(const nlohmann::json& json) {
  return Quote{
      common::safe_string_from_json_array(json, 0), // ticker
      common::safe_string_from_json_array(json, 1), // description
      common::safe_string_from_json_array(json, 2), // type
      common::safe_string_from_json_array(json, 3), // exchange
      common::safe_int_from_json_array(json, 4),    // total_rank
      common::safe_double_from_json_array(json, 5), // last_close_price
      common::safe_string_from_json_array(json, 6), // currency
      common::safe_double_from_json_array(json, 7), // close_change_24h
      common::safe_double_from_json_array(json, 8), // market_cap_calc
      common::safe_double_from_json_array(json, 9), // vol_cmc_24h
      common::safe_double_from_json_array(json, 10) // circulating_supply
  };
}
