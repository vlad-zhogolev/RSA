#include <iostream>
#include "BigUnsignedInt.h"
#include "RandomGenerator.h"

using namespace std;

const BigUnsignedInt::size_type SIZE = 256;//1024;
/*
0       1001111010110101100111110001110111100101
1       1001111010110101100111110001110111100101
2       1000000011111001101010011101000111000111
3       1000001111001011110000011001101110101101
4       1101110100101001010000001100100011000101
5       1111111011011110000000111100101001010111
6       1100010001000011010001000001000000111001
7       1010100000011011111101110001001110100101
*/
int main()
{
    RandomGenerator rnd(4081u,	25673u,	121500u, 1u);
    BigUnsignedInt random = rnd.next(SIZE);
    BigUnsignedInt::size_type i = 0;
    while(!random.isPrime())
    {
        cout << i++ << "\t" << random << endl;
        random = rnd.next(SIZE);
    }
    cout << i << "\t" << random << endl;
    return 0;
}