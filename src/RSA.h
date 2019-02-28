//
// Created by ZV on 28.02.2019.
//

#ifndef BIGUNSIGNEDINTBASE10_RSA_H
#define BIGUNSIGNEDINTBASE10_RSA_H

#include <fstream>
#include <string_view>
#include "BigUnsignedInt.h"

class RSA {
    void encodeFile(std::string_view input, std::string_view output, const BigUnsignedInt& e, const BigUnsignedInt& n);

    void decodeFile(std::string_view input, std::string_view output, const BigUnsignedInt& d, const BigUnsignedInt& n);
};


#endif //BIGUNSIGNEDINTBASE10_RSA_H
