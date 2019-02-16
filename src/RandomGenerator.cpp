//
// Created by ZV on 15.02.2019.
//

#include <stdexcept>
#include "RandomGenerator.h"

using namespace std;

RandomGenerator::RandomGenerator(Unsigned a, Unsigned c, Unsigned m, Unsigned x)
{
    if (a >= m || c >= m || x >= m)
        throw invalid_argument("Parameters of random generator must be less than specified module");
}

BigUnsignedIntBase10 RandomGenerator::next(BigUnsignedIntBase10::size_type size)
{
    using size_type = BigUnsignedIntBase10::size_type;

    if (size < 2)
        throw invalid_argument("Generated number must have at least 2 digits");

    BigUnsignedIntBase10 number(size);
    number[0] = number[size - 1] = 1;
    for (size_type i = 1; i < size; ++i)
    {
        _cur = (_a * _cur + _c) % _m;
        number[i] = _cur < _m / 2 ? 0 : 1;
    }

    return number;
}