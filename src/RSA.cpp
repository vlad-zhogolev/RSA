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
        throw invalid_argument("Can't open output file");

    fin.seekg(0, fin.end);
    size_t length = fin.tellg();
    fin.seekg(0, fin.beg);
    const BigUnsignedInt::size_type bufferSize = n.length() / CHAR_BIT + (n.length() % CHAR_BIT > 0 ? 1 : 0);
    size_t tail = length % bufferSize;
    BigUnsignedInt message(n.length());
    size_t counter = 0;
    char byte;

    fin >> noskipws;
    fout << length / bufferSize << " " << tail << "\n";
    while (fin >> byte)
    {
        for (size_t i = 0; i < CHAR_BIT; ++i)
            message[i + counter * CHAR_BIT] = (byte & (1u << i)) > 0 ? 1 : 0;
        ++counter;

        if (counter == bufferSize)
        {
            message.countAndSetSignificantDigits();
            BigUnsignedInt encrypted = message.pow(e, n);
            auto bytes = encrypted.toBytes(bufferSize);
            for (char b:bytes)
            {
                fout << b;
                fout.flush();
            }
            counter = 0;
        }
    }

    if (counter > 0)
    {
        for (size_t i = counter * CHAR_BIT; i < message.length(); ++i)
            message[i] = 0;

        message.countAndSetSignificantDigits();
        BigUnsignedInt encrypted = message.pow(e, n);
        auto bytes = encrypted.toBytes(bufferSize);
        for (char b:bytes)
        {
            fout << b;
            fout.flush();
        }
    }
}

void RSA::decodeFile(std::string_view input, std::string_view output, const BigUnsignedInt& d, const BigUnsignedInt& n)
{
    ifstream fin(input.data(), ios_base::binary);
    ofstream fout(output.data(), ios_base::binary);

    if (!fin)
        throw invalid_argument("Can't open input file");
    if (!fout)
        throw invalid_argument("Can't open output file");

    const BigUnsignedInt::size_type bufferSize = n.length() / CHAR_BIT + (n.length() % CHAR_BIT > 0 ? 1 : 0);
    BigUnsignedInt message(n.length());
    size_t counter = 0;
    char byte;

    size_t length, tail;
    fin >> length >> tail;
    fin >> noskipws;
    fin >> byte;
    while (fin >> byte)
    {
        for (size_t i = 0; i < CHAR_BIT; ++i)
            message[i + counter * CHAR_BIT] = (byte & (1u << i)) > 0 ? 1 : 0;
        ++counter;

        if (counter == bufferSize && length-- > 0)
        {
            message.countAndSetSignificantDigits();
            BigUnsignedInt encrypted = message.pow(d, n);
            auto bytes = encrypted.toBytes(bufferSize);
            for (char b:bytes)
            {
                fout << b;
                fout.flush();
            }
            counter = 0;
        }
    }

    if (counter > 0)
    {
        for (size_t i = counter * CHAR_BIT; i < message.length(); ++i)
            message[i] = 0;

        message.countAndSetSignificantDigits();
        BigUnsignedInt encrypted = message.pow(d, n);
        auto bytes = encrypted.toBytes(tail);
        for (BigUnsignedInt::size_type i = 0; i < tail; ++i)
        {
            fout << bytes[i];
            fout.flush();
        }
    }
}
