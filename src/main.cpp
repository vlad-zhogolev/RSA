#include <iostream>
#include "BigUnsignedInt.h"
#include "RandomGenerator.h"

using namespace std;

const BigUnsignedInt::size_type SIZE = 512;//1024;

int main()
{
    RandomGenerator rnd(4081u,	25673u,	121500u, 0u);
    BigUnsignedInt random = rnd.next(SIZE);
    BigUnsignedInt::size_type i = 0;
    while(!random.isPrime())
    {
        cout << ++i << endl;
        random = rnd.next(SIZE);
    }
    cout << random << endl;
    return 0;
}