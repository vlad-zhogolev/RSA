#include <iostream>
#include <fstream>
#include "BigUnsignedInt.h"
#include "RandomGenerator.h"
#include "RSA.h"

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
    os << "Checked " << p.second << " numbers, before found prime:" << "\n" << p.first;
}

int main()
{
    BigUnsignedInt message(SIZE);
    ifstream fin("input.txt");

    RandomGenerator rg(4081u, 25673u, 121500u, 0u);

    auto p = findPrimeWithCounter(SIZE, rg);
    //auto p = make_pair(
    //        BigUnsignedInt("10101111011001110010101000010010011101001100111111110110010101010010011010110010"
    //                               "111001001000010010000101101111111000010111101111"), SIZE);
    cout << endl << p << endl;

    auto q = findPrimeWithCounter(SIZE, rg);
    //auto q = make_pair(
    //        BigUnsignedInt("11100101011010001011101101101111000100101000111001000100010101110100001001000001"
    //                               "000000111111111011100000000101110001110011011001"), SIZE);
    cout << endl << q << endl;

    BigUnsignedInt n = p.first * q.first;
    BigUnsignedInt fi = (p.first - one) * (q.first - one);
    BigUnsignedInt e("100000001");// 257
    BigUnsignedInt d = e.multInverse(fi);

    RSA::encodeFile("input.txt", "encoded.txt", e, n);

    RSA::decodeFile("encoded.txt", "decoded.txt", d, n);
    return 0;
}