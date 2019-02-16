include_directories(../libs/googletest/include)

add_executable(runBigUnsignedTests BigUnsignedTests.cpp ../../src/BigUnsignedIntBase10.cpp)

target_link_libraries(runBigUnsignedTests gtest gtest_main)
