//
// Created by ZV on 22.09.2018.
//

#include "BigUnsignedInt.h"

using namespace std;

std::istream& operator>>(std::istream& is, BigUnsignedInt& bigUnsignedInt)
{
    // Check that provided input is unsigned number
    string input;
    is >> input;
    BigUnsignedInt::createFromString(input, bigUnsignedInt);
    return is;
}

std::ostream& operator<<(std::ostream& os, const BigUnsignedInt& number)
{
    for (auto it = number._digits.rbegin() + number.startIndex();
         it != number._digits.rend(); ++it)
        os << static_cast<unsigned char>(*it + '0');
    return os;
}

std::ostream& operator<<(std::ostream& os, BigUnsignedInt&& bigUnsignedInt)
{
    os << bigUnsignedInt;
    return os;
}

BigUnsignedInt operator+(const BigUnsignedInt& a, const BigUnsignedInt& b)
{
    BigUnsignedInt result(a);
    return result += b;
}

BigUnsignedInt operator-(const BigUnsignedInt& a, const BigUnsignedInt& b)
{
    BigUnsignedInt result(a);
    return result -= b;
}

BigUnsignedInt operator*(const BigUnsignedInt& a, const BigUnsignedInt& b)
{
    using size_type = BigUnsignedInt::size_type;
    using Digit = BigUnsignedInt::size_type;

    BigUnsignedInt result(a._digitsNumber + b._digitsNumber);

    for (size_type j = 0; j < b._digitsNumber; ++j)
    {
        size_type k = 0;
        for (size_type i = 0; i < a._digitsNumber; ++i)
        {
            Digit t = a._digits[i] * b._digits[j] + result._digits[i + j] + k;
            result._digits[i + j] = t % a._base;
            k = t / a._base;
        }
        result._digits[j + a._digitsNumber] = k;
    }

    result._digitsNumber = result.countSignificantDigits();

    /*if (result._digits[result._digitsNumber - 1] == 0)
        --result._digitsNumber;
*/
    return result;
}

BigUnsignedInt operator/(const BigUnsignedInt& a, const BigUnsignedInt& b)
{
    return a.quotientAndRem(b).first;
}

std::pair<BigUnsignedInt, BigUnsignedInt>
quotientAndRem(const BigUnsignedInt& a, const BigUnsignedInt& b)
{
    return a.quotientAndRem(b);
}

bool operator<(const BigUnsignedInt& a, const BigUnsignedInt& b)
{
    if (a._digitsNumber != b._digitsNumber)
        return a._digitsNumber < b._digitsNumber;

    return lexicographical_compare(a.startIter(), a._digits.crend(), b.startIter(), b._digits.crend());
}

bool operator>(const BigUnsignedInt& a, const BigUnsignedInt& b) { return b < a; }

bool operator<=(const BigUnsignedInt& a, const BigUnsignedInt& b) { return !(b < a); }

bool operator>=(const BigUnsignedInt& a, const BigUnsignedInt& b) { return !(a < b); }

bool operator==(const BigUnsignedInt& a, const BigUnsignedInt& b)
{
    // TODO: write tests
    using size_type = BigUnsignedInt::size_type;

    if (a._digitsNumber != b._digitsNumber)
        return false;

    for (size_type i = 0; i < a._digitsNumber; ++i)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

bool operator!=(const BigUnsignedInt& a, const BigUnsignedInt& b) { return !(a == b); }

BigUnsignedInt& BigUnsignedInt::operator+=(const BigUnsignedInt& other)
{
    size_type requiredNumberOfDigits = max(_digitsNumber, other._digitsNumber) + 1;

    if (size() <= requiredNumberOfDigits)
        resize(requiredNumberOfDigits);

    Digit carry = 0;
    // Sum significant digits from other with this considering carry
    transform(
            _digits.begin(), _digits.begin() + other._digitsNumber,
            other._digits.begin(), _digits.begin(),
            [&carry, this](Digit a, Digit b) {
                // TODO: come up with more conscious name for type "Digit"
                a += b + carry;
                carry = a < _base ? 0 : 1;
                // sum can't exceed _base * 2, so this line is equivalent for sum % _base but should be faster
                return a < _base ? a : a - _base;
            });

    auto it = _digits.begin() + other._digitsNumber;
    // Promote carry
    while (carry != 0)
    {
        *it += carry;
        carry = *it < _base ? 0 : 1;
        *it = *it < _base ? *it : *it - _base;
        ++it;
    }
    _digitsNumber = max(static_cast<size_type>(it - _digits.begin()), _digitsNumber);

    return *this;
}

BigUnsignedInt& BigUnsignedInt::operator-=(const BigUnsignedInt& other)
{
    size_type length = min(_digitsNumber, other._digitsNumber);

    Digit k = 1; // Tracks if we engaged from next digit (1: no engage, 0: engaged)
    std::transform(
            _digits.begin(), _digits.begin() + length,
            other._digits.begin(), _digits.begin(),
            [&k, this](Digit a, Digit b) {
                // We always perform an engage from next digit.
                a += _max_digit - b + k;
                // Check if engage was necessary (a is in [0, _base) ) and set appropriate k
                k = a < _base ? 0 : 1;
                return a < _base ? a : a - _base;
            });

    auto it = _digits.begin() + length;
    // Engage while necessary
    while (k == 0)
    {
        if (*it == 0)
            *it = _max_digit;
        else
        {
            --(*it);
            k = 1;
        }
        ++it;
    }

    // Set appropriate _digitNumber
    _digitsNumber = countSignificantDigits();

    return *this;
}

BigUnsignedInt& BigUnsignedInt::operator*=(const BigUnsignedInt& other)
{
    return *this = *this * other;
}

BigUnsignedInt& BigUnsignedInt::operator/=(const BigUnsignedInt& other)
{
    *this = *this / other;
}

std::pair<BigUnsignedInt, BigUnsignedInt>
BigUnsignedInt::quotientAndRem(const BigUnsignedInt& other) const
{
    if (other == zero)
        throw invalid_argument("Division by zero");

    if (other == one)
        return make_pair(*this, zero);

    if (*this < other)
        return make_pair(zero, *this);

    // Just to get short names
    size_type m = _digitsNumber - other._digitsNumber;
    size_type n = other._digitsNumber;

    BigUnsignedInt quotient(m + 1);

    BigUnsignedInt u(size() + 1u);
    copy(_digits.begin(), _digits.end(), u._digits.begin());
    u._digitsNumber = size();

    // Won't work if other has only one digit
    auto condition = [v = other[n - 2]](Digit q, Digit r, Digit u) {
        return q == _base || q * v > _base * r + u;
    };

    for (size_type j = m; j != numeric_limits<size_type>::max(); --j)
    {
        Digit estimQuotient = u[j + n] * _base + u[j + n - 1];

        if (condition(estimQuotient, 0u /*estimReminder*/, u[j + n - 2]))
        {
            --estimQuotient;
            // Add test for this in division
            if (condition(estimQuotient, 1u /*estimReminder*/, u[j + n - 2]))
                --estimQuotient;
        }

        BigUnsignedInt uPart(u._digits.begin() + j, u._digits.begin() + j + n + 1);
        BigUnsignedInt q(1u);
        q[0] = estimQuotient;

        BigUnsignedInt qv = q * other;
        if (uPart < qv)
        {
            --estimQuotient;
            qv -= other;
        }
        uPart -= qv;

        copy(uPart._digits.begin(), uPart._digits.end(), u._digits.begin() + j);
        fill(u._digits.begin() + j + uPart._digitsNumber, u._digits.begin() + j + n + 1, 0);
        quotient[j] = estimQuotient;
    }

    if (quotient[quotient._digitsNumber - 1] == 0 && quotient._digitsNumber > 1)
        --quotient._digitsNumber;

    u._digitsNumber = u.countSignificantDigits();
    return make_pair(quotient, u);
}

BigUnsignedInt& BigUnsignedInt::operator%=(const BigUnsignedInt& module)
{
    if (module == zero)
        throw invalid_argument("Division by zero");

    // TODO: decide how to manage case when module is 1
    /*if (module == one)
        return zero;*/

    if (*this < module)
        return *this;

    // Just to get short names
    size_type m = _digitsNumber - module._digitsNumber;
    size_type n = module._digitsNumber;

    // Won't work if other has only one digit
    auto condition = [v = module[n - 2]](Digit q, Digit r, Digit u) {
        return q == _base || q * v > _base * r + u;
    };

    // First iteration performed as if there is additional digit
    // Additional digit is actually zero
    Digit estimQuotientFirst = _digits[m + n - 1];

    if (condition(estimQuotientFirst, 0u /*estimReminder*/, _digits[m + n - 2]))
    {
        --estimQuotientFirst;
        // Add test for this in division
        if (condition(estimQuotientFirst, 1u /*estimReminder*/, _digits[m + n - 2]))
            --estimQuotientFirst;
    }

    // Copy will take one less digit
    BigUnsignedInt uPartFirst(_digits.begin() + m, _digits.begin() + m + n);
    BigUnsignedInt qFirst(1u);
    qFirst[0] = estimQuotientFirst;

    BigUnsignedInt qvFirst = qFirst * module;
    if (uPartFirst < qvFirst)
        qvFirst -= module;
    uPartFirst -= qvFirst;

    copy(uPartFirst._digits.begin(), uPartFirst._digits.end(), _digits.begin() + m);
    fill(_digits.begin() + m + uPartFirst._digitsNumber, _digits.begin() + m + n, 0);
    for (size_type j = m - 1; j != numeric_limits<size_type>::max(); --j)
    {
        Digit estimQuotient = _digits[j + n] * _base + _digits[j + n - 1];

        if (condition(estimQuotient, 0u /*estimReminder*/, _digits[j + n - 2]))
        {
            --estimQuotient;
            // Add test for this in division
            if (condition(estimQuotient, 1u /*estimReminder*/, _digits[j + n - 2]))
                --estimQuotient;
        }

        BigUnsignedInt uPart(_digits.begin() + j, _digits.begin() + j + n + 1);
        BigUnsignedInt q(1u);
        q[0] = estimQuotient;

        BigUnsignedInt qv = q * module;
        if (uPart < qv)
            qv -= module;
        uPart -= qv;

        copy(uPart._digits.begin(), uPart._digits.end(), _digits.begin() + j);
        fill(_digits.begin() + j + uPart._digitsNumber, _digits.begin() + j + n + 1, 0);
    }

    _digitsNumber = countSignificantDigits();
    return *this;
}

BigUnsignedInt BigUnsignedInt::pow(const BigUnsignedInt& degree) const
{
    if (degree._base != 2)
        throw invalid_argument("Degree base must be 2");

    BigUnsignedInt x(*this);
    BigUnsignedInt result(one);
    for (Digit d:degree._digits)
    {
        if (d == 1)
            result *= x;
        x *= x;
    }
    return result;
}

BigUnsignedInt BigUnsignedInt::pow(const BigUnsignedInt& degree, const BigUnsignedInt& module) const
{
    if (degree._base != 2)
        throw invalid_argument("Degree base must be 2");

    BigUnsignedInt x(*this);
    BigUnsignedInt result(one);

    for (Digit d:degree._digits)
    {
        if (d == 1)
        {
            result = result * x;
            result %= module;
        }
        x = x * x;
        x %= module;
    }
    return result;
}

BigUnsignedInt BigUnsignedInt::multInverse(const BigUnsignedInt& v)
{
    BigUnsignedInt inv;
    BigUnsignedInt t1;
    int iter;
    /* Step X1. Initialise */
    BigUnsignedInt u1(one);
    BigUnsignedInt v1(zero);

    BigUnsignedInt u3(*this);
    //BigUnsignedInt v1(_base, "0");
    BigUnsignedInt v3(v);
    /* Remember odd/even iterations */
    iter = 1;
    /* Step X2. Loop while v3 != 0 */
    while (v3 != zero)
    {
        /* Step X3. Divide and "Subtract" */
        auto res = u3.quotientAndRem(v3);
        t1 = u1 + res.first * v1;
        /* Swap */
        u1 = v1;
        v1 = t1;
        u3 = v3;
        v3 = res.second;
        iter = -iter;
    }
    /* Make sure u3 = gcd(u,v) == 1 */
    if (u3 != one)
        return zero;   /* Error: No inverse exists */
    /* Ensure a positive result */
    if (iter < 0)
        return v - u1;
    else
        return u1;
}

bool BigUnsignedInt::isPrime() const
{
    size_type k = 1;
    while (k < _digitsNumber && _digits[k] == 0)
        ++k;

    // TODO: consider this when writing tests
    if (k >= _digitsNumber)
        throw invalid_argument("Degree of n-1 is invalid");

    BigUnsignedInt m(_digits.begin() + k, _digits.end());
    BigUnsignedInt test = *this - one;

    BigUnsignedInt t = two.pow(m, *this);

    if (t == one || t == test)
        return true;
    for (size_type i = 0; i < k; ++i)
    {
        t = (t * t).quotientAndRem(*this).second;
        if (t == one)
            return false;
        if (t == test)
            return true;
    }
    return false;
}

BigUnsignedInt::size_type BigUnsignedInt::countSignificantDigits()
{
    size_type zeroesNumber = 0;
    size_type numberOfDigits = 0;
    for (auto it = _digits.begin(); it != _digits.end(); ++it)
    {
        if (*it == 0)
            ++zeroesNumber;
        else
        {
            numberOfDigits += zeroesNumber + 1;
            zeroesNumber = 0;
        }
    }

    return max(numberOfDigits, 1u);
}

std::pair<BigUnsignedInt, BigUnsignedInt> BigUnsignedInt::quotientAndRem(Digit d) const
{
    Digit r = 0; // Reminder
    BigUnsignedInt quotient(_digitsNumber);
    for (size_type j = _digitsNumber - 1; j != numeric_limits<size_type>::max(); --j)
    {
        Digit sum = r * _base + _digits[j];
        r = sum % d;
        quotient[j] = sum / d;
    }

    quotient._digitsNumber = quotient.countSignificantDigits();
    return std::make_pair(quotient, BigUnsignedInt(std::to_string(r)));
}

BigUnsignedInt::BigUnsignedInt(std::string_view str)
{
    createFromString(str, *this);
}

BigUnsignedInt::BigUnsignedInt(UnsignedVector::const_iterator b,
                               UnsignedVector::const_iterator e)
{
    for (auto it = b; it != e; ++it)
    {
        if (*it >= _base)
            throw invalid_argument("Digits in range must be less than base");
    }
    _digits = UnsignedVector(b, e);
    _digitsNumber = countSignificantDigits();
}

std::string BigUnsignedInt::to_string() const
{
    stringstream ss;
    for (auto it = _digits.rbegin() + startIndex(); it != _digits.rend(); ++it)
        ss << *it;
    return ss.str();
}

void BigUnsignedInt::resetToZero()
{
    for (Digit& digit:_digits)
        digit = 0;
    _digitsNumber = 1;
}

void BigUnsignedInt::createFromString(string_view str, BigUnsignedInt& number)
{
    using namespace std;

    // Check input
    regex unsignedNumber("[[:digit:]]+");
    if (!regex_match(str.data(), unsignedNumber))
        throw invalid_argument("provided input isn't unsigned integer");

    number._digits = UnsignedVector(str.size());
    // Fill dividend with digits of provided number
    transform(str.rbegin(), str.rend(), number._digits.begin(), [](string::value_type letter) {
        return static_cast<Digit>(letter - '0');
    });
    number._digitsNumber = number.countSignificantDigits();

    //TODO: make check before or during transformation
    for (auto it = number._digits.begin(); it != number._digits.end(); ++it)
    {
        if (*it >= number._base)
            throw invalid_argument("Digits in string must be less than base");

    }
}

void BigUnsignedInt::resize(size_type size) { _digits.resize(size, 0); }