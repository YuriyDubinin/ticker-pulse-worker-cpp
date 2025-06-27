#include "quote_model.h"
#include "common_utils.h"

Quote Quote::from_json(const nlohmann::json& json) {
  return Quote{
      common_utils::safe_string_from_json_array(json, 0), // ticker
      common_utils::safe_string_from_json_array(json, 1), // description
      common_utils::safe_string_from_json_array(json, 2), // type
      common_utils::safe_string_from_json_array(json, 3), // exchange
      common_utils::safe_int_from_json_array(json, 4),    // total_rank
      common_utils::safe_double_from_json_array(json, 5), // last_close_price
      common_utils::safe_string_from_json_array(json, 6), // currency
      common_utils::safe_double_from_json_array(json, 7), // close_change_24h
      common_utils::safe_double_from_json_array(json, 8), // market_cap_calc
      common_utils::safe_double_from_json_array(json, 9), // vol_cmc_24h
      common_utils::safe_double_from_json_array(json, 10) // circulating_supply
  };
}
