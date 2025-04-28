set (CMAKE_CXX_STANDARD_23)
project(a3a)


find_package (Seastar REQUIRED)

add_executable (abc
  abc.cc)

add_executable (seastarlink
  seastarlink.cc)



target_link_libraries(abc
  Seastar::seastar)
target_link_libraries (seastarlink
  Seastar::seastar)


enable_testing()
FILE(GLOB UnitTests_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*_test.cpp" )
ADD_EXECUTABLE(UnitTester test_runner.cpp ${UnitTests_SRCS})
FOREACH(test ${UnitTests_SRCS})
        GET_FILENAME_COMPONENT(TestName ${test} NAME_WE)
        add_test(${TestName} UnitTester ${TestName})
ENDFOREACH(test)

target_link_libraries (UnitTester cppunit)


