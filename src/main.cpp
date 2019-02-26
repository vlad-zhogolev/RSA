#include <iostream>
#include "BigUnsignedInt.h"
#include "RandomGenerator.h"

using namespace std;

int main()
{
    RandomGenerator rnd(4081u,	25673u,	121500u, 0u);
    BigUnsignedInt random = rnd.next(SIZE);
    BigUnsignedInt::size_type i = 0;
    while(!random.isPrime())
    {
        cout << i++ << "\t" << random << endl;
        random = rnd.next(SIZE);
    }
    BigUnsignedInt first = random;

    i = 0;
    random = rnd.next(SIZE);
    while(!random.isPrime())
    {
        cout << i++ << "\t" << random << endl;
        random = rnd.next(SIZE);
    }
    cout << endl << "First prime:" << endl << i << "\t" << first << endl << endl << endl;
    cout << endl << "Second prime:" << endl << i << "\t" << random << endl;
    return 0;
}