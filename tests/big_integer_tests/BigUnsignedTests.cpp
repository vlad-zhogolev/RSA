#include "gtest/gtest.h"
#include "../../src/BigUnsignedInt.h"

using namespace std;

using threeStrings = vector<tuple<string, string, string>>;
using fourStrings = vector<tuple<string, string, string, string>>;
using twoStringsOneBool = vector<tuple<string, string, bool>>;
using oneStringOneBool = vector<tuple<string, bool>>;

const BigUnsignedInt::Digit TWO = 2;

threeStrings addTestData{
        {
                "0",
                "0",
                "0"
        },
        {       "0",
                "1",
                "1"
        },
        {       "1",
                "0",
                "1"
        },
        {       "1001",
                "1",
                "1010"
        },
        {       "1100010",
                "11",
                "1100101"
        },
        {       "11",
                "1100010",
                "1100101"
        },
        {       "10111101",
                "1101001",
                "100100110"
        },
        {       "101111101011110000011111111",
                "1",
                "101111101011110000100000000"
        },
        {       "10000000000000000000000000000000000000000000000000000000000000000",
                "10000000000000000000000000000000000000000000000000000000000000000",
                "100000000000000000000000000000000000000000000000000000000000000000"
        },
        {       "10000011011000101001001101000011110100111101110011010001110000000000000000000000",
                "111100000111001001001011110000010100100010000010100000110101101110010101011011000",
                "1001100100010001110010101011000110011001001110000111011000011101110010101011011000"
        }
};

TEST(Addition, Test_01)
{
    for (const auto& d:addTestData)
    {
        BigUnsignedInt a( get<0>(d));
        BigUnsignedInt b( get<1>(d));
        BigUnsignedInt expected( get<2>(d));
        BigUnsignedInt c = a + b;
        EXPECT_EQ(c, expected);
    }
}

threeStrings subtractTestData{
        {
                "0",
                "0",
                "0"
        },
        {       "1",
                "0",
                "1"
        },
        {       "1010",
                "1000",
                "10"
        },
        {       "1100100",
                "1100001",
                "11"
        },
        {       "1111101000",
                "1",
                "1111100111"
        },
        {       "1010010101110",
                "111000000010",
                "11010101100"
        },
        {       "100000000000000000000000000000000000000000000000000000000000000000",
                "10000000000000000000000000000000000000000000000000000000000000000",
                "10000000000000000000000000000000000000000000000000000000000000000"
        },
        {       "1001100100010001110010101011000110011001001110000111011000011101110010101011011000",
                "111100000111001001001011110000010100100010000010100000110101101110010101011011000",
                "10000011011000101001001101000011110100111101110011010001110000000000000000000000"}};


TEST(Subtraction, Test_01)
{
    for (const auto& d:subtractTestData)
    {
        BigUnsignedInt a( get<0>(d));
        BigUnsignedInt b(get<1>(d));
        BigUnsignedInt expected( get<2>(d));
        BigUnsignedInt c = a - b;
        EXPECT_EQ(c, expected);
    }
}

threeStrings multiplyTestData{
        {
                "0",
                "0",
                "0"
        },
        {       "1",
                "0",
                "0"
        },
        {       "0",
                "1",
                "0"
        },
        {       "1",
                "1",
                "1"
        },
        {       "10",
                "1",
                "10"
        },
        {       "1",
                "10",
                "10"
        },
        {       "101",
                "110",
                "11110"
        },
        {       "1111011",
                "111001000",
                "1101101100011000"
        },
        {       "1000110010101101100111000011000011110000101111010110010",
                "101000100101110100110010001101011010011001110001001101101",
                "101100100111001000100111000000000011010000010001111110100011000110011000011001100110001101100010111010111001010"
        },
        {       "1101010100011011110110100001010100001010101110100101110000",
                "100100110001011101101100101000111101000001001010011111100110000",
                "111101001110010011111100010101011001011001011100101100111110000011110100000110100111011110010100101011100101010100000000"
        }
};

TEST(Multiplication, Test_01)
{
    for (const auto& d:multiplyTestData)
    {
        BigUnsignedInt a(get<0>(d));
        BigUnsignedInt b(get<1>(d));
        BigUnsignedInt expected( get<2>(d));
        BigUnsignedInt c = a * b;
        EXPECT_EQ(c, expected);
    }
}

fourStrings divisionTestData{
        {
                "0",
                "1",
                "0",
                "0"
        },
        {       "0",
                "10100",
                "0",
                "0"
        },
        {       "0",
                "100111101000101011000101",
                "0",
                "0"
        },
        {       "11111100100101001",
                "1001001111110100111100001",
                "0",
                "11111100100101001"
        },
        {       "1",
                "1",
                "1",
                "0"
        },
        {       "1010",
                "10",
                "101",
                "0"
        },
        {       "1100100",
                "10101",
                "100",
                "10000"
        },
        {       "10010001",
                "1101",
                "1011",
                "10"
        },
        {       "11011000",
                "110100",
                "100",
                "1000"
        },
        {       "11000011010100000",
                "1100100",
                "1111101000",
                "0"
        },
        {       "1011001001110010001001110000000000110100000100011111101000110001100110"
                        "00011001100110001101100010111010111001010",
                "1000110010101101100111000011000011110000101111010110010",
                "101000100101110100110010001101011010011001110001001101101",
                "0"
        },
        {       "1011001001110010001001110000000000110100000100011111101000110001100110"
                        "00011001100110001101100010111010111001010",
                "100010111011011001011110011001101100011011111",
                "1010001101111100100001011101111000001111010101111000001111011101101",
                "10000111110011111001110111101000110101010111"
        },
        {       "1111010011100100111111000101010110010110010111001011001111100000111101"
                        "00000110100111011110010100101011100101010100000000",
                "1101010100011011110110100001010100001010101110100101110000",
                "100100110001011101101100101000111101000001001010011111100110000",
                "0"
        }
};

TEST(Division, Test_01)
{
    for (const auto& d:divisionTestData)
    {
        BigUnsignedInt a( get<0>(d));
        BigUnsignedInt b( get<1>(d));
        BigUnsignedInt q( get<2>(d));
        BigUnsignedInt r( get<3>(d));
        auto result = a.quotientAndRem(b);
        EXPECT_EQ(result.first, q);
        EXPECT_EQ(result.second, r);
    }
}

TEST(Mod, Test_01)
{
    for (const auto& d:divisionTestData)
    {
        BigUnsignedInt a( get<0>(d));
        BigUnsignedInt b( get<1>(d));
        BigUnsignedInt r( get<3>(d));
        EXPECT_EQ(a.mod(b), r);
    }
}

threeStrings multInverseTestData{
        {"111", "1101",           "10"},
        {"11",  "1101011",        "100100"},
        {"11",  "10011110000101", "1101001011001"}
};

TEST(MultInverse, Test_01)
{
    for (const auto& d:multInverseTestData)
    {
        BigUnsignedInt a( get<0>(d));
        BigUnsignedInt module( get<1>(d));
        BigUnsignedInt multInverse = a.multInverse(module);
        BigUnsignedInt expected( get<2>(d));
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
        BigUnsignedInt a(get<0>(d));
        BigUnsignedInt degree( get<1>(d));
        BigUnsignedInt expected( get<2>(d));
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
        BigUnsignedInt a( get<0>(d));
        BigUnsignedInt degree( get<1>(d));
        BigUnsignedInt module( get<2>(d));
        BigUnsignedInt expected( get<3>(d));
        EXPECT_EQ(a.pow(degree, module), expected);
    }
}

// Third element holds 'true' if first is less than second and 'false' otherwise.
twoStringsOneBool lessComparisonTestData{
        {
                "0",
                "0",

                false
        },
        {       "1",
                "0",
                false
        },
        {       "0",
                "1",
                true
        },
        {       "1100100",
                "1",
                false
        },
        {       "1",
                "1100100",
                true
        },
        {       "101100100111001000100111000000000011010000010001111110100011000110011000011001100110001101100010111010111001010",
                "1000110010101101100111000011000011110000101111010110010",
                false}
};

TEST(LessComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedInt a(get<0>(d));
        BigUnsignedInt b(get<1>(d));
        EXPECT_EQ(a < b, get<2>(d));
    }
}

TEST(GreaterComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedInt a(get<0>(d));
        BigUnsignedInt b(get<1>(d));
        EXPECT_EQ(b > a, get<2>(d));
    }
}

TEST(LessOrEqualComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedInt a(get<0>(d));
        BigUnsignedInt b( get<1>(d));
        EXPECT_EQ(b <= a, !get<2>(d));
    }
}

TEST(GreaterOrEqualComparison, Test_01)
{
    for (const auto& d:lessComparisonTestData)
    {
        BigUnsignedInt a( get<0>(d));
        BigUnsignedInt b( get<1>(d));
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
        BigUnsignedInt a(get<0>(d));
        EXPECT_EQ(a.isPrime(), get<1>(d));
    }
}