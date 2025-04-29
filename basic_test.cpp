#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include "jsonifyext.hpp"

namespace ns {
struct ComplexStruct {
  std::variant<std::string, int> id;
  std::optional<std::string> x;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ComplexStruct, x, id);

}
class basic_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( basic_test );
  CPPUNIT_TEST( testEmpty );
  CPPUNIT_TEST( testJsonWithVariant );
  CPPUNIT_TEST_SUITE_END();
public:
  void testEmpty () { int i=5; CPPUNIT_ASSERT(i==5); }
  void testJsonWithVariant () {
    {
      using json = nlohmann::json;
      ns::ComplexStruct comp{ "x" };
      json r = comp;
      std::cout << r.dump() << std::endl;
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
};
CPPUNIT_TEST_SUITE_REGISTRATION( basic_test );
