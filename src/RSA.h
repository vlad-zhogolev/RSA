//
// Created by ZV on 28.02.2019.
//

#ifndef BIGUNSIGNEDINTBASE10_RSA_H
#define BIGUNSIGNEDINTBASE10_RSA_H


#include <string_view>

class RSA {
    void encodeFile(std::string_view input, std::string_view output);

    void decodeFile(std::string_view input, std::string_view output);
};


#endif //BIGUNSIGNEDINTBASE10_RSA_H
