#ifndef a3a_request_hpp
#define a3a_request_hpp
#include <string>
#include "jsonifyext.hpp"


namespace a3a {

  using namespace std;
  using namespace nlohmann;

  string TOOLS_LIST = "tools/list";
  string TOOLS_CALL = "tools/call";

  class annotations_item {
  public:
    optional<string> title;
    optional<bool> readOnlyHint;
    optional<bool> destructiveHint;
    optional<bool> idempotentHint;
    optional<bool> openWorldHint;
  };
  NLOHMANN_JSONIFY_ALL_THINGS(annotations_item, readOnlyHint, destructiveHint, idempotentHint, openWorldHint);


  class input_schema {
  public:
    string type;
    json properties;
    vector<string> required;
  };
  NLOHMANN_JSONIFY_ALL_THINGS(input_schema, type, properties, required);


  class tool {
  public:
    string name;
    optional<string> description;
    input_schema inputSchema;
    optional<annotations_item> annotations;
  };
  NLOHMANN_JSONIFY_ALL_THINGS(tool, name, description, inputSchema, annotations);

  class call_tool_result {

    bool isError;
  };

  class implementation {
  public:
    string name;
    string version;
  };
  //changed class name to not clash with propert
  class sampling_item{
  public:
  };
  class root_capabilities {
  public:
    bool listChanged;
  };
  class client_capabilities{
  public:
    nlohmann::json experimental;
    root_capabilities roots;
    sampling_item sampling;
  };
  class initialize_reqest {
  public:
    string protocolVersion;
    client_capabilities capabilities;
    implementation clientInfo;
  };

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


  class resource_read_response {
  public:
    std::vector<resource_read> contents;

  };
  NLOHMANN_JSONIFY_ALL_THINGS(resource_read_response, contents);

}
#endif
