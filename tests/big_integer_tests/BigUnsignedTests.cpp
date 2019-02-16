#include "gtest/gtest.h"
#include "../../src/BigUnsignedIntBase10.h"

using namespace std;

using TestData = vector<tuple<string, string, string>>;
using DivisionTestData = vector<tuple<string, string, string, string>>;
using ComparisonTestData = vector<tuple<string, string, bool>>;

BigUnsignedIntBase10::Digit base = 10;

TestData addTestData{
        {"0",                        "0",                         "0"},
        {"0",                        "1",                         "1"},
        {"1",                        "0",                         "1"},
        {"9",                        "1",                         "10"},
        {"98",                       "3",                         "101"},
        {"3",                        "98",                        "101"},
        {"189",                      "105",                       "294"},
        {"99999999",                 "1",                         "100000000"},
        {"18446744073709551616",     "18446744073709551616",      "36893488147419103232"},
        {"620448401733239439360000", "2270952686874132953377496", "2891401088607372392737496"}};

TestData subtractTestData{
        {"0",                         "0",                        "0"},
        {"1",                         "0",                        "1"},
        {"10",                        "8",                        "2"},
        {"100",                       "97",                       "3"},
        {"1000",                      "1",                        "999"},
        {"5294",                      "3586",                     "1708"},
        {"36893488147419103232",      "18446744073709551616",     "18446744073709551616"},
        {"2891401088607372392737496", "620448401733239439360000", "2270952686874132953377496"}};

TestData multiplyTestData{
        {"0",                  "0",                   "0"},
        {"1",                  "0",                   "0"},
        {"0",                  "1",                   "0"},
        {"1",                  "1",                   "1"},
        {"2",                  "1",                   "2"},
        {"1",                  "2",                   "2"},
        {"5",                  "6",                   "30"},
        {"123",                "456",                 "56088"},
        {"19798691543539378",  "91402832913949293",   "1809656495068950591261389240759754"},
        {"239939174036859248", "5299529849434881840", "1271564814857086600480923129591256320"}};

DivisionTestData divisionTestData{
        {"0",                                     "1",                  "0",                    "0"},
        {"0",                                     "20",                 "0",                    "0"},
        {"0",                                     "10390213",           "0",                    "0"},
        {"129321",                                "19392993",           "0",                    "129321"},
        {"1",                                     "1",                  "1",                    "0"},
        {"10",                                    "2",                  "5",                    "0"},
        {"100",                                   "21",                 "4",                    "16"},
        {"145",                                   "13",                 "11",                   "2"},
        {"216",                                   "52",                 "4",                    "8"},
        {"1809656495068950591261389240759754",    "19798691543539378",  "91402832913949293",    "0"},
        {"1809656495068950591261389240759754",    "19201923012831",     "94243503312648019693", "9332861078871"},
        {"1271564814857086600480923129591256320", "239939174036859248", "5299529849434881840",  "0"}
};

TestData multInverseTestData{
        {"7", "13",    "2"},
        {"3", "107",   "36"},
        {"3", "10117", "6745"}
};

// Third element holds 'true' if first is less than second and 'false' otherwise.
ComparisonTestData lessComparisonTestData{
        {"0",                                  "0",                 false},
        {"1",                                  "0",                 false},
        {"0",                                  "1",                 true},
        {"100",                                "1",                 false},
        {"1",                                  "100",               true},
        {"1809656495068950591261389240759754", "19798691543539378", false}
};

TEST(Addition, Test_01)
{
    for (const auto& d:addTestData)
    {
        BigUnsignedIntBase10 a(get<0>(d));
        BigUnsignedIntBase10 b(get<1>(d));
        BigUnsignedIntBase10 expected(get<2>(d));
        BigUnsignedIntBase10 c = a + b;
        EXPECT_EQ(c, expected);
    }
}

TEST(Subtraction, Test_01)
{
    for (const auto& d:subtractTestData)
    {
        BigUnsignedIntBase10 a(get<0>(d));
        BigUnsignedIntBase10 b(get<1>(d));
        BigUnsignedIntBase10 expected(get<2>(d));
        BigUnsignedIntBase10 c = a - b;
        EXPECT_EQ(c, expected);
    }
}

TEST(Multiplication, Test_01)
{
    for (const auto& d:multiplyTestData)
    {
        BigUnsignedIntBase10 a(get<0>(d));
        BigUnsignedIntBase10 b(get<1>(d));
        BigUnsignedIntBase10 expected(get<2>(d));
        BigUnsignedIntBase10 c = a * b;
        EXPECT_EQ(c, expected);
    }
}

TEST(Division, Test_01)
{
    for (const auto& d:divisionTestData)
    {
        BigUnsignedIntBase10 a(get<0>(d));
        BigUnsignedIntBase10 b(get<1>(d));
        BigUnsignedIntBase10 q(get<2>(d));
        BigUnsignedIntBase10 r(get<3>(d));
        auto result = a.quotientAndMod(b);
        EXPECT_EQ(result.first, q);
        EXPECT_EQ(result.second, r);
    }
}

TEST(MultInverse, Test_01)
{
    for (const auto& d:multInverseTestData)
    {
        BigUnsignedIntBase10 a(get<0>(d));
        BigUnsignedIntBase10 module(get<1>(d));
        BigUnsignedIntBase10 multInverse = a.multInverse(module);
        BigUnsignedIntBase10 expected(get<2>(d));
        EXPECT_EQ(multInverse, expected);
    }
}

TEST(LessComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedIntBase10 a(get<0>(d));
        BigUnsignedIntBase10 b(get<1>(d));
        EXPECT_EQ(a < b, get<2>(d));
    }
}

TEST(GreaterComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedIntBase10 a(get<0>(d));
        BigUnsignedIntBase10 b(get<1>(d));
        EXPECT_EQ(b > a, get<2>(d));
    }
}

TEST(LessOrEqualComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedIntBase10 a(get<0>(d));
        BigUnsignedIntBase10 b(get<1>(d));
        EXPECT_EQ(b <= a, !get<2>(d));
    }
}

TEST(GreaterOrEqualComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedIntBase10 a(get<0>(d));
        BigUnsignedIntBase10 b(get<1>(d));
        EXPECT_EQ(a >= b, !get<2>(d));
    }
}