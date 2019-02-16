#include <iostream>
#include "BigUnsignedIntBase10.h"
#include "RandomGenerator.h"

using namespace std;

int main()
{
    BigUnsignedIntBase10 a("7");
    BigUnsignedIntBase10 b("13");

    RandomGenerator rnd(859u, 2531u, 11979u, 0u);
    BigUnsignedIntBase10 c = rnd.next(512);
    //BigUnsignedIntBase10 a("100");
    //BigUnsignedIntBase10 b("10");
    //std::cout << a << endl << b << endl << a.multInverse(b) << endl;
    std::cout << c;
    return 0;
}