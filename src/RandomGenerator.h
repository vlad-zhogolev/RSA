//
// Created by ZV on 15.02.2019.
//

#ifndef BIGUNSIGNEDINTBASE10_RANDOMGENERATOR_H
#define BIGUNSIGNEDINTBASE10_RANDOMGENERATOR_H

#include "BigUnsignedIntBase10.h"

/**
 * Generates numbers from {0, 1} using linear congruent method.
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
     *
     * @param size
     * @return
     */
    BigUnsignedIntBase10 next(BigUnsignedIntBase10::size_type size);

private:
    Unsigned _a;
    Unsigned _c;
    Unsigned _m;
    Unsigned _cur;
    Unsigned _x0;
};


#endif //BIGUNSIGNEDINTBASE10_RANDOMGENERATOR_H
