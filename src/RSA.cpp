//
// Created by ZV on 28.02.2019.
//

#include "RSA.h"

using namespace std;

void RSA::encodeFile(std::string_view input, std::string_view output, const BigUnsignedInt& e, const BigUnsignedInt& n)
{
    ifstream fin(input.data(), ios_base::binary);
    ofstream fout(output.data(), ios_base::binary);

    if (!fin)
        throw invalid_argument("Can't open input file");
    if (!fout)
        throw invalid_argument("Can't open ouptut file");

    fin.seekg(0, fin.end);
    size_t length = fin.tellg();
    fin.seekg(0, fin.beg);
    const BigUnsignedInt::size_type bufferSize = e.length() / CHAR_BIT;
    size_t tailLength = length % bufferSize;
    length -= tailLength;
    char byte;

    size_t counter = 0;
    BigUnsignedInt message(e.length());
    while (length-- > 0)
    {
        fin >> byte;
        if(counter < bufferSize)
        {
            for (size_t i = 0; i < CHAR_BIT; ++i)
                message[i + counter * CHAR_BIT] = (byte & (1u << i)) > 0 ? 1 : 0;
            ++counter;
        }
        else
        {
            BigUnsignedInt encrypted = message.pow(e, n);
            for(char b:encrypted.toBytes(bufferSize))
                fout << b;
            counter = 0;
        }
    }
}

void RSA::decodeFile(std::string_view input, std::string_view output, const BigUnsignedInt& d, const BigUnsignedInt& n)
{

}
