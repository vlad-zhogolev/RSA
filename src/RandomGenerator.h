//
// Created by ZV on 15.02.2019.
//

#ifndef BIGUNSIGNEDINTBASE10_RANDOMGENERATOR_H
#define BIGUNSIGNEDINTBASE10_RANDOMGENERATOR_H

#include "BigUnsignedInt.h"

/**
 * Generates binary numbers using linear congruent method.
 * Generated numbers have first and last digits set to 1 to provide to
 * guarantee that numbers are sufficiently large and could be prime.
 */
class RandomGenerator {
public:
    using Unsigned = unsigned int;
public:
    /**
     * Creates instance with specified multiplier, increment, module and
     * start value.
     * @param a - multiplier
     * @param c - increment
     * @param m - module
     * @param x - start value
     */
    RandomGenerator(Unsigned a, Unsigned c, Unsigned m, Unsigned x);

    /**
     * Generates next number of specified size.
     * @param size - size of number in bits
     * @return - generated number.
     */
    BigUnsignedInt next(BigUnsignedInt::size_type size);

private:
    Unsigned _a;
    Unsigned _c;
    Unsigned _m;
    Unsigned _cur;
    Unsigned _x0;
};


#endif //BIGUNSIGNEDINTBASE10_RANDOMGENERATOR_H
