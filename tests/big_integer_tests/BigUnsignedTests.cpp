#include "gtest/gtest.h"
#include "../../src/BigUnsignedInt.h"

using namespace std;

using threeStrings = vector<tuple<string, string, string>>;
using fourStrings = vector<tuple<string, string, string, string>>;
using twoStringsOneBool = vector<tuple<string, string, bool>>;
using oneStringOneBool = vector<tuple<string, bool>>;

const BigUnsignedInt::Digit TEN = 10;
const BigUnsignedInt::Digit TWO = 2;

threeStrings addTestData{
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

TEST(Addition, Test_01)
{
    for (const auto& d:addTestData)
    {
        BigUnsignedInt a(TEN, get<0>(d));
        BigUnsignedInt b(TEN, get<1>(d));
        BigUnsignedInt expected(TEN, get<2>(d));
        BigUnsignedInt c = a + b;
        EXPECT_EQ(c, expected);
    }
}

threeStrings subtractTestData{
        {"0",                         "0",                        "0"},
        {"1",                         "0",                        "1"},
        {"10",                        "8",                        "2"},
        {"100",                       "97",                       "3"},
        {"1000",                      "1",                        "999"},
        {"5294",                      "3586",                     "1708"},
        {"36893488147419103232",      "18446744073709551616",     "18446744073709551616"},
        {"2891401088607372392737496", "620448401733239439360000", "2270952686874132953377496"}};


TEST(Subtraction, Test_01)
{
    for (const auto& d:subtractTestData)
    {
        BigUnsignedInt a(TEN, get<0>(d));
        BigUnsignedInt b(TEN, get<1>(d));
        BigUnsignedInt expected(TEN, get<2>(d));
        BigUnsignedInt c = a - b;
        EXPECT_EQ(c, expected);
    }
}

threeStrings multiplyTestData{
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

TEST(Multiplication, Test_01)
{
    for (const auto& d:multiplyTestData)
    {
        BigUnsignedInt a(TEN, get<0>(d));
        BigUnsignedInt b(TEN, get<1>(d));
        BigUnsignedInt expected(TEN, get<2>(d));
        BigUnsignedInt c = a * b;
        EXPECT_EQ(c, expected);
    }
}

fourStrings divisionTestData{
        {"0",                                     "1",                  "0",                    "0"},
        {"0",                                     "20",                 "0",                    "0"},
        {"0",                                     "10390213",           "0",                    "0"},
        {"129321",                                "19392993",           "0",                    "129321"},
        {"1",                                     "1",                  "1",                    "0"},
        {"10",                                    "2",                  "5",                    "0"},
        {"100",                                   "21",                 "4",                    "16"},
        {"145",                                   "13",                 "11",                   "2"},
        {"216",                                   "52",                 "4",                    "8"},
        {"100000",                                "100",                "1000",                 "0"},
        {"1809656495068950591261389240759754",    "19798691543539378",  "91402832913949293",    "0"},
        {"1809656495068950591261389240759754",    "19201923012831",     "94243503312648019693", "9332861078871"},
        {"1271564814857086600480923129591256320", "239939174036859248", "5299529849434881840",  "0"}
};

TEST(Division, Test_01)
{
    for (const auto& d:divisionTestData)
    {
        BigUnsignedInt a(TEN, get<0>(d));
        BigUnsignedInt b(TEN, get<1>(d));
        BigUnsignedInt q(TEN, get<2>(d));
        BigUnsignedInt r(TEN, get<3>(d));
        auto result = a.quotientAndMod(b);
        EXPECT_EQ(result.first, q);
        EXPECT_EQ(result.second, r);
    }
}

threeStrings multInverseTestData{
        {"7", "13",    "2"},
        {"3", "107",   "36"},
        {"3", "10117", "6745"}
};

TEST(MultInverse, Test_01)
{
    for (const auto& d:multInverseTestData)
    {
        BigUnsignedInt a(TEN, get<0>(d));
        BigUnsignedInt module(TEN, get<1>(d));
        BigUnsignedInt multInverse = a.multInverse(module);
        BigUnsignedInt expected(TEN, get<2>(d));
        EXPECT_EQ(multInverse, expected);
    }
}

threeStrings powTestData{
        {"0",   "0",          "1"},
        {"1",   "0",          "1"},
        {"10",  "111",        "10000000"},
        {"1",   "1101011011", "1"},
        {"10",  "10",         "100"},
        {"101", "11",         "1111101"}
};

TEST(Pow, Test_01)
{
    for (const auto& d:powTestData)
    {
        BigUnsignedInt a(TWO, get<0>(d));
        BigUnsignedInt degree(TWO, get<1>(d));
        BigUnsignedInt expected(TWO, get<2>(d));
        EXPECT_EQ(a.pow(degree), expected);
    }
}

fourStrings modularPowTestData{
        {"0",   "0",          "1",    "1"},
        {"0",   "0",          "10",   "1"},
        {"0",   "1",          "10",   "0"},
        {"1",   "0",          "10",   "1"},
        {"10",  "111",        "1000", "0"},
        {"1",   "1101011011", "10",   "1"},
        {"10",  "10",         "11",   "1"},
        {"101", "11",         "111",  "110"}
};

TEST(ModularPow, Test_01)
{
    for (const auto& d:modularPowTestData)
    {
        BigUnsignedInt a(TWO, get<0>(d));
        BigUnsignedInt degree(TWO, get<1>(d));
        BigUnsignedInt module(TWO, get<2>(d));
        BigUnsignedInt expected(TWO, get<3>(d));
        EXPECT_EQ(a.pow(degree, module), expected);
    }
}

// Third element holds 'true' if first is less than second and 'false' otherwise.
twoStringsOneBool lessComparisonTestData{
        {"0",                                  "0",                 false},
        {"1",                                  "0",                 false},
        {"0",                                  "1",                 true},
        {"100",                                "1",                 false},
        {"1",                                  "100",               true},
        {"1809656495068950591261389240759754", "19798691543539378", false}
};

TEST(LessComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedInt a(TEN, get<0>(d));
        BigUnsignedInt b(TEN, get<1>(d));
        EXPECT_EQ(a < b, get<2>(d));
    }
}

TEST(GreaterComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedInt a(TEN, get<0>(d));
        BigUnsignedInt b(TEN, get<1>(d));
        EXPECT_EQ(b > a, get<2>(d));
    }
}

TEST(LessOrEqualComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedInt a(TEN, get<0>(d));
        BigUnsignedInt b(TEN, get<1>(d));
        EXPECT_EQ(b <= a, !get<2>(d));
    }
}

TEST(GreaterOrEqualComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedInt a(TEN, get<0>(d));
        BigUnsignedInt b(TEN, get<1>(d));
        EXPECT_EQ(a >= b, !get<2>(d));
    }
}

oneStringOneBool primarityTestData{
        {"111",  true},
        {"1101", true},
        {"110",  false},
        {"1000", false},
        {"1001", false}
};

TEST(IsPrime, Test_01)
{
    for (const auto& d:primarityTestData)
    {
        BigUnsignedInt a(TWO, get<0>(d));
        EXPECT_EQ(a.isPrime(), get<1>(d));
    }
}