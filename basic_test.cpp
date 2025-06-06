#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include "jsonifyext.hpp"
#include "request.h"

namespace ns {
struct ComplexStruct {
  std::variant<std::string, int> id;
  std::optional<std::string> x;
};

NLOHMANN_JSONIFY_ALL_THINGS(ComplexStruct, x, id);

}
class basic_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( basic_test );
  CPPUNIT_TEST( testEmpty );
  CPPUNIT_TEST( testJsonWithVariant );
  CPPUNIT_TEST( testModelSerialization );
  CPPUNIT_TEST( toolSerializationTest );
  CPPUNIT_TEST_SUITE_END();
public:
  void testEmpty () { int i=5; CPPUNIT_ASSERT(i==5); }
  void testJsonWithVariant () {
    {
      using json = nlohmann::json;
      ns::ComplexStruct comp{ "x" };
      json r = comp;
      std::cout << r.dump() << std::endl;
      ns::ComplexStruct p2 = r.template get<ns::ComplexStruct>();r.template get<ns::ComplexStruct>();

      CPPUNIT_ASSERT("x" == std::get<std::string> (p2.id)  );
      CPPUNIT_ASSERT(r.at("id") == "x");
    }
    {
      using json = nlohmann::json;
      ns::ComplexStruct comp{ 5 };
      json r = comp;
      std::cout << r.dump() << std::endl;
      CPPUNIT_ASSERT(r.at("id") == 5);
    }
  }
  void testModelSerialization(){
    {
      using json = nlohmann::json;
      a3a::request r;
      r.jsonrpc = "yo";
      r.id = "56";
      json j = r;

      auto p2 = j.template get<a3a::request>();
      std::cout << j.dump() << std::endl;
    }

    {
      using json = nlohmann::json;
      a3a::request_json_params r;
      r.jsonrpc = "yo";
      r.id = 56;
      r.params = json::parse(R"(
        [ {"x":"y"},{"a":"b"}]
      )");
      json j = r;

      std::cout << j.dump() << std::endl;
      a3a::request_json_params p2 = j.template get<a3a::request_json_params>();
      std::cout << "params " << p2.params << std::endl;
      std::cout << "ddf " << p2.jsonrpc.value() << std::endl;
    }
  }

  void toolSerializationTest(){
    using json = nlohmann::json;
    a3a::tool t;
    t = json::parse(R"({"name": "calculate_sum",
"description": "Add two numbers together",
"inputSchema": {
"type": "object",
"properties": {
"a": { "type": "number" },
"b": { "type": "number" }
},
"required": ["a", "b"]
}
})");
    CPPUNIT_ASSERT(t.name == "calculate_sum");
    std::cout << t.inputSchema.properties << std::endl;
  }
};
CPPUNIT_TEST_SUITE_REGISTRATION( basic_test );
