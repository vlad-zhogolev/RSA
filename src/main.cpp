#include <iostream>
#include <fstream>
#include "BigUnsignedInt.h"
#include "RandomGenerator.h"

using namespace std;

// Size of key in bytes
const BigUnsignedInt::size_type SIZE_IN_BYTES = 16;
const BigUnsignedInt::size_type SIZE = CHAR_BIT * SIZE_IN_BYTES;

/**
 * Finds prime BigUnsignedInt with high probability.
 * @param size - Size of number in bits.
 * @param rg - RandomGenerator instance.
 * @return - Pair (prime, count), where counter holds how many numbers where checked for primarity.
 */
pair<BigUnsignedInt, size_t> findPrimeWithCounter(BigUnsignedInt::size_type size, RandomGenerator& rg)
{
    size_t count = 0;
    BigUnsignedInt number = rg.next(size);
    while (!number.isPrime())
    {
        cout << ".";
        ++count;
        number = rg.next(size);
    }
    return make_pair(number, count);
}

std::ostream& operator<<(ostream& os, const pair<BigUnsignedInt, size_t>& p)
{
    os << "Checked " << p.second << " numbers, before found prime: " << p.first;
}

int main()
{
    BigUnsignedInt message(SIZE);
    ifstream fin("input.txt");

    char byte;
    size_t counter = 0;
    while (counter < SIZE_IN_BYTES && fin >> byte)
    {
        for (size_t i = 0; i < CHAR_BIT; ++i)
            message[i + counter * CHAR_BIT] = (byte & (1u << i)) > 0 ? 1 : 0;
        ++counter;
    }
    cout << message << endl;

    RandomGenerator rg(4081u, 25673u, 121500u, 0u);
    auto p = findPrimeWithCounter(SIZE, rg);
    cout << endl << p << endl;
    auto q = findPrimeWithCounter(SIZE, rg);
    cout << endl << q << endl;

    BigUnsignedInt n = p.first * q.first;
    BigUnsignedInt fi = (p.first - one) * (q.first - one);
    BigUnsignedInt e("100000001");// 257
    BigUnsignedInt d = e.multInverse(fi);

    BigUnsignedInt encoded = message.pow(e, n);
    BigUnsignedInt decoded = encoded.pow(d, n);

    cout << decoded;
    return 0;
}