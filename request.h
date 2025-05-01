#ifndef a3a_request_hpp
#define a3a_request_hpp
#include <string>
#include "jsonifyext.hpp"
#include <nlohmann/json.hpp>

namespace a3a {
  using namespace std;
  using namespace nlohmann;

  class request{
  public:
    std::optional<string> jsonrpc;
    std::optional<variant<string, int>> id;
    std::optional<string> method;
  };
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(request, jsonrpc, id, method);

  class request_json_params : public request {
  public:
    std::optional<nlohmann::json> params;
  };
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(request_json_params, jsonrpc, id, method, params);

  template <class Item>
  class request_typed_params: public request {
    optional<Item> params;
  };
  /** this requires template deduction registration impossible */
  //NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(request_typed_params, jsonrpc, id, method, params);
}
#endif
