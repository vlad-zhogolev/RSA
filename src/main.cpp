#include <iostream>
#include "BigUnsignedInt.h"
#include "RandomGenerator.h"

using namespace std;

int main()
{
    BigUnsignedInt a(10, "7");
    BigUnsignedInt b(10, "13");

    RandomGenerator rnd(859u, 2531u, 11979u, 0u);
    BigUnsignedInt c = rnd.next(512);
    //BigUnsignedInt a("100");
    //BigUnsignedInt b("10");
    std::cout << a << endl << b << endl <<  c << endl;
    //std::cout << c;
    return 0;
}