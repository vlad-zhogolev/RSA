include_directories(../libs/googletest/include)

add_executable(runBigUnsignedTests BigUnsignedTests.cpp ../../src/BigUnsignedInt.cpp ../../src/RandomGenerator.cpp)

target_link_libraries(runBigUnsignedTests gtest gtest_main)
