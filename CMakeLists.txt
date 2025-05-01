set (CMAKE_CXX_STANDARD_23)
project(a3a)

find_package (Seastar REQUIRED)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable (abc abc.cc)
add_executable (seastarlink seastarlink.cc)
add_executable(jsonusage jsonusage.cc jsonifyext.hpp request.h)

target_link_libraries(jsonusage nlohmann_json::nlohmann_json)
target_link_libraries(abc Seastar::seastar nlohmann_json::nlohmann_json)
target_link_libraries (seastarlink Seastar::seastar nlohmann_json::nlohmann_json)


enable_testing()
FILE(GLOB UnitTests_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*_test.cpp" )
ADD_EXECUTABLE(UnitTester test_runner.cpp ${UnitTests_SRCS})
FOREACH(test ${UnitTests_SRCS})
        GET_FILENAME_COMPONENT(TestName ${test} NAME_WE)
        add_test(${TestName} UnitTester ${TestName})
ENDFOREACH(test)

target_link_libraries (UnitTester cppunit Seastar::seastar nlohmann_json::nlohmann_json )


