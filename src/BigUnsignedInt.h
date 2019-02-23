//
// Created by ZV on 28.09.2018.
//

#ifndef BIGUNSIGNEDINTEGER_BIGUNSIGNEDINT_H
#define BIGUNSIGNEDINTEGER_BIGUNSIGNEDINT_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <regex>
#include <queue>

class BigUnsignedInt;

/**
 * Template of input operator for BigUnsignedInt of size N. Expects
 * a number in decimal system as input.
 * @tparam N - size of BigUnsignedInt to read
 * @param is - input stream from which BigUnsignedInt will be read
 * @param bigUnsignedInt - object to store number read
 * @return - reference to is
 */
std::istream& operator>>(std::istream& is, BigUnsignedInt& bigUnsignedInt);

/**
 * Template of output operator for BigUnsignedInt of size N. Outputs
 * binary representation of provided object.
 * @tparam N - size of BigUnsignedInt to write
 * @param os - output stream
 * @param number - object to write to stream
 * @return - reference to os
 */
std::ostream& operator<<(std::ostream& os, const BigUnsignedInt& number);

/**
 * Template of output operator for BigUnsignedInt rvalues of size N.
 * Outputs binary representation of provided object.
 * @tparam N - size of BigUnsignedInt to write
 * @param os - output stream
 * @param bigUnsignedInt - object to write to stream
 * @return - reference to os
 */
std::ostream& operator<<(std::ostream& os, BigUnsignedInt&& bigUnsignedInt);

/**
 * Template of shift to the left operator for BigUnsignedInt of size N.
 * @tparam N - size of BigUnsignedInt
 * @param bigUnsignedInt - object to shift
 * @param shift - number of bits to shift
 * @return - copy of bigUnsignedInt shifted by the provided number of bits
 */

/*BigUnsignedInt operator<<(BigUnsignedInt bigUnsignedInt, std::size_t shift);*/

/**
 * Template of operator+ for BigUnsignedInt of size N.
 * @tparam N - size of BigUnsignedInt
 * @param a - first operand
 * @param b - second operand
 * @return - sum of first and second operands
 */
BigUnsignedInt operator+(const BigUnsignedInt& a, const BigUnsignedInt& b);

/**
 * Template of operator- for BigUnsignedInt of size N.
 * @tparam N - size of BigUnsignedInt
 * @param a - first operand
 * @param b - second operand
 * @return - difference between first and second operands
 */
BigUnsignedInt operator-(const BigUnsignedInt& a, const BigUnsignedInt& b);

/**
 * Tempalate of operator* for BigUnsignedInt of size N.
 * @tparam N - size of BigUnsignedInt
 * @param a - first operand
 * @param b - second operand
 * @return - product of first and second operands
 */
BigUnsignedInt operator*(const BigUnsignedInt& a, const BigUnsignedInt& b);

/**
 *
 * @param a
 * @param b
 * @return
 */
BigUnsignedInt operator/(const BigUnsignedInt& a, const BigUnsignedInt& b);

/**
 *
 * @param a
 * @param b
 * @return
 */
std::pair<BigUnsignedInt, BigUnsignedInt>
quotientAndRem(const BigUnsignedInt& a, const BigUnsignedInt& b);

/**
 *
 * @param a
 * @param b
 * @return
 */
bool operator<(const BigUnsignedInt& a, const BigUnsignedInt& b);

bool operator>(const BigUnsignedInt& a, const BigUnsignedInt& b);

bool operator<=(const BigUnsignedInt& a, const BigUnsignedInt& b);

bool operator>=(const BigUnsignedInt& a, const BigUnsignedInt& b);

bool operator==(const BigUnsignedInt& a, const BigUnsignedInt& b);

bool operator!=(const BigUnsignedInt& a, const BigUnsignedInt& b);

/**
 *
 * @param a
 * @param b
 * @return
 */
BigUnsignedInt MultInverse(const BigUnsignedInt& a, const BigUnsignedInt& b);

/**
 * BigUnsignedInt class represents unsigned integer which has N bits
 * in binary representation.
 * @tparam N - number of bits in unsigned integer
 */
class BigUnsignedInt {
public:

    ///
    /// Type aliases
    ///
    using Digit = unsigned char;
    using UnsignedVector = std::vector<Digit>;
    using size_type = UnsignedVector::size_type;

    ///
    /// Constants
    ///
    static const Digit _base = 2u;
    static const Digit _max_digit = _base - 1;

    ///
    /// Friends
    ///
    friend std::istream& operator>>(std::istream& is, BigUnsignedInt& bigUnsignedInt);

    friend std::ostream& operator<<(std::ostream& os, const BigUnsignedInt& bigUnsignedInt);

    friend std::ostream& operator<<(std::ostream& os, BigUnsignedInt&& bigUnsignedInt);

    friend bool operator<(const BigUnsignedInt& a, const BigUnsignedInt& b);

    friend bool operator==(const BigUnsignedInt& a, const BigUnsignedInt& b);

    friend BigUnsignedInt operator*(const BigUnsignedInt& a, const BigUnsignedInt& b);

    friend std::pair<BigUnsignedInt, BigUnsignedInt>
    quotientAndRem(const BigUnsignedInt& a, const BigUnsignedInt& b);

public:

    ///\brief Constructs unsigned integer with zero value.
    BigUnsignedInt() : _digits(1), _digitsNumber(1) {}

    /**\brief Creates BigUnsignedInt object from string representation.
     * Throws same exceptions as createFromString for the same reasons.
     * @param stringRepr - string containing an unsigned number
     */
    explicit BigUnsignedInt(std::string_view str);

    /**
     * \brief Creates BigUnisgnedInt object capable to hold size digits.
     * @param size - quantity of digits in this number.
     */
    explicit BigUnsignedInt(size_type size) : _digits(size, 0), _digitsNumber(size) {}

    /**
     * Creates number from specified range.
     * @param b - iterator to the least significant digit.
     * @param e - iterator to past the most significant digit.
     */
    BigUnsignedInt(UnsignedVector::const_iterator b, UnsignedVector::const_iterator e);

    /**\brief Increases this BigUnsignedInt by other.
     * @param other - BigUnsignedInt to add
     * @return - reference to *this object increased by other
     */
    BigUnsignedInt& operator+=(const BigUnsignedInt& other);

    /**\brief Decreases this BigUnsignedInt by other.
     * @param other - BigUnsignedInt to subtract
     * @return - reference to *this object decreased by other
     */
    BigUnsignedInt& operator-=(const BigUnsignedInt& other);

    /**\brief Multiplies this BigUnsignedInt by other.
     * @param other - BigUnsignedInt to multiply on
     * @return - reference to *this object multiplied by other
     */
    BigUnsignedInt& operator*=(const BigUnsignedInt& other);

    /**
     * \brief Divides this BigUnsignedInt by other.
     * @param other - BigUnsignedInt to divide by.
     * @return - reference to *this object divided by other
     */
    BigUnsignedInt& operator/=(const BigUnsignedInt& other);

    /**
     * \brief Divides this BigUnsignedInt by other, returning quotient and reminder.
     * This function modifies *this by storing quotient in it.
     * @param other - BigUnsignedInt to divide by.
     * @return - pair (quotient, reminder), where quotient is reference to *this object
     * and reminder is ordinary BigUnsignedInt.
     */
    std::pair<BigUnsignedInt, BigUnsignedInt> quotientAndRem(const BigUnsignedInt& other) const;

    BigUnsignedInt& mod(const BigUnsignedInt& module);

    /**
     *
     * @param degree
     * @return
     */
    BigUnsignedInt pow(const BigUnsignedInt& degree) const;

    /**
     *
     * @param degree
     * @param mod
     * @return
     */
    BigUnsignedInt pow(const BigUnsignedInt& degree, const BigUnsignedInt& mod) const;

    /**
     *
     * @param a
     * @return
     */
    BigUnsignedInt multInverse(BigUnsignedInt a);

    bool isPrime() const;

    /**
     * \brief Provides access to digits of the number by index.
     * @param index - position of digit in number
     * @return - Reference to digit in specified position.
     */
    UnsignedVector::value_type& operator[](size_type index) { return _digits[index]; }

    /**
     * \brief Provides access to digits of the number by index.
     * @param index - position of digit in number
     * @return - Reference to digit in specified position.
     */
    const UnsignedVector::value_type& operator[](size_type index) const { return _digits[index]; }

    /**
     * @return String representation of number
     */
    std::string to_string() const;

private:

    /**
     * \brief Resets number to zero
     */
    void resetToZero();

    /**
     * Stores binary representation of number contained in string in provided object.
     * Throws invalid argument exception if provided string contains something
     * else but digits, size of BigUnsignedInt is too small to represent provided number.
     * @param input - string containing an unsigned number
     * @param number - object to store the number converted from string
     */
    static void createFromString(std::string_view str, BigUnsignedInt& number);

    /**
     * Sets the _digitsNumber with appropriate number.
     */
    size_type countSignificantDigits();

    /**
     * Performs division of *this on digit d.
     * @param d - digit to divide on.
     * @return - pair (quotient, reminder)
     */
    std::pair<BigUnsignedInt, BigUnsignedInt> quotientAndRem(Digit d) const;

    /**
     * \breif Resizes number so it can store specified number of digits
     * @param size - new required number of digits
     */
    void resize(size_type size);

    /**
     * @return - Number of digits which can be stored without resize in this number.
     */
    inline size_type size() const { _digits.size(); }

    /**
     * @return - Index of the most significant digit counting from end of _digits.
     */
    inline size_type startIndex() const { return size() - _digitsNumber; }

    /**
     * @return - reverse iterator to the position of the most significant digit.
     */
    UnsignedVector::reverse_iterator startIter() { return _digits.rbegin() + startIndex(); }

    /**
     * @return - constant reverse iterator to the position of the most significant digit.
     */
    UnsignedVector::const_reverse_iterator startIter() const { return _digits.crbegin() + startIndex(); }

private:
    UnsignedVector _digits; // Stores digit of the number in reverse order (little-endian)
    size_type _digitsNumber; // number of significant digits
};

static const BigUnsignedInt zero;
static const BigUnsignedInt one("1");
static const BigUnsignedInt two("10");

#endif //BIGUNSIGNEDINTEGER_BIGUNSIGNEDINT_H
