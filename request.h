#ifndef a3a_request_hpp
#define a3a_request_hpp
#include <string>
#include "jsonifyext.hpp"

namespace a3a {
  using namespace std;
  using namespace nlohmann;

  class request{
  public:
    std::optional<string> jsonrpc;
    std::optional<variant<string, int>> id;
    std::optional<string> method;
  };
  NLOHMANN_JSONIFY_ALL_THINGS(request, jsonrpc, id, method);

  class request_json_params : public request {
  public:
    nlohmann::json params;
  };
  NLOHMANN_JSONIFY_ALL_THINGS(request_json_params, jsonrpc, id, method, params);

  template <class Item>
  class request_typed_params: public request {
  public:
    optional<Item> params;
  };
  /** this requires template deduction registration impossible */
  //NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(request_typed_params, jsonrpc, id, method, params);

  class resource {
    public:
      string uri;
      string name;
      optional<string> description;
      optional<string> mimeType;
  };
  NLOHMANN_JSONIFY_ALL_THINGS(resource, uri, name, description, mimeType);

  class resource_read {
  public:
    string uri;
    optional<string> mimeType;
    optional<string> text;
    optional<string> blob; // For binary resources (base64 encoded)
  };
  NLOHMANN_JSONIFY_ALL_THINGS(resource_read, uri, mimeType, text, blob);
}
#endif
