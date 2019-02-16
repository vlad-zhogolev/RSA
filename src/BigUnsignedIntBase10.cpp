//
// Created by ZV on 22.09.2018.
//

#include "BigUnsignedIntBase10.h"

using namespace std;

std::istream& operator>>(std::istream& is, BigUnsignedIntBase10& bigUnsignedInt)
{
    // Check that provided input is unsigned number
    string input;
    is >> input;
    BigUnsignedIntBase10::createFromString(input, bigUnsignedInt);
    return is;
}

std::ostream& operator<<(std::ostream& os, const BigUnsignedIntBase10& number)
{
    for (auto it = number._digits.rbegin() + number.startIndex();
         it != number._digits.rend(); ++it)
        os << *it;
    return os;
}

std::ostream& operator<<(std::ostream& os, BigUnsignedIntBase10&& bigUnsignedInt)
{
    os << bigUnsignedInt;
    return os;
}

BigUnsignedIntBase10 operator+(const BigUnsignedIntBase10& a, const BigUnsignedIntBase10& b)
{
    BigUnsignedIntBase10 result(a);
    return result += b;
}

BigUnsignedIntBase10 operator-(const BigUnsignedIntBase10& a, const BigUnsignedIntBase10& b)
{
    BigUnsignedIntBase10 result(a);
    return result -= b;
}

BigUnsignedIntBase10 operator*(const BigUnsignedIntBase10& a, const BigUnsignedIntBase10& b)
{
    using size_type = BigUnsignedIntBase10::size_type;
    using Digit = BigUnsignedIntBase10::size_type;

    BigUnsignedIntBase10 result(a._digitsNumber + b._digitsNumber);

    for (size_type j = 0; j < b._digitsNumber; ++j)
    {
        size_type k = 0;
        for (size_type i = 0; i < a._digitsNumber; ++i)
        {
            Digit t = a._digits[i] * b._digits[j] + result._digits[i + j] + k;
            result._digits[i + j] = t % BigUnsignedIntBase10::_base;
            k = t / BigUnsignedIntBase10::_base;
        }
        result._digits[j + a._digitsNumber] = k;
    }

    if (result._digits[result._digitsNumber - 1] == 0)
        --result._digitsNumber;

    return result;
}

BigUnsignedIntBase10 operator/(const BigUnsignedIntBase10& a, const BigUnsignedIntBase10& b)
{
    return a.quotientAndMod(b).first;
}

std::pair<BigUnsignedIntBase10, BigUnsignedIntBase10>
quotientAndMod(const BigUnsignedIntBase10& a, const BigUnsignedIntBase10& b)
{
    return a.quotientAndMod(b);
}

bool operator<(const BigUnsignedIntBase10& a, const BigUnsignedIntBase10& b)
{
    if (a._digitsNumber != b._digitsNumber)
        return a._digitsNumber < b._digitsNumber;

    return lexicographical_compare(a.startIter(), a._digits.crend(), b.startIter(), b._digits.crend());
}

bool operator>(const BigUnsignedIntBase10& a, const BigUnsignedIntBase10& b) { return b < a; }

bool operator<=(const BigUnsignedIntBase10& a, const BigUnsignedIntBase10& b) { return !(b < a); }

bool operator>=(const BigUnsignedIntBase10& a, const BigUnsignedIntBase10& b) { return !(a < b); }

bool operator==(const BigUnsignedIntBase10& a, const BigUnsignedIntBase10& b)
{
    // TODO: write tests
    using size_type = BigUnsignedIntBase10::size_type;

    if (a._digitsNumber != b._digitsNumber)
        return false;

    for (size_type i = 0; i < a._digitsNumber; ++i)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

bool operator!=(const BigUnsignedIntBase10& a, const BigUnsignedIntBase10& b) { return !(a == b); }

BigUnsignedIntBase10& BigUnsignedIntBase10::operator+=(const BigUnsignedIntBase10& other)
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

BigUnsignedIntBase10& BigUnsignedIntBase10::operator-=(const BigUnsignedIntBase10& other)
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
    _digitsNumber = countSignificantNumbers();

    return *this;
}

BigUnsignedIntBase10& BigUnsignedIntBase10::operator*=(const BigUnsignedIntBase10& other)
{
    return *this = *this * other;
}

BigUnsignedIntBase10& BigUnsignedIntBase10::operator/=(const BigUnsignedIntBase10& other)
{
    *this = *this / other;
}

std::pair<BigUnsignedIntBase10, BigUnsignedIntBase10>
BigUnsignedIntBase10::quotientAndMod(const BigUnsignedIntBase10& other) const
{
    if (*this < other)
        return make_pair(BigUnsignedIntBase10("0"), *this);

    if (other._digitsNumber == 1)
        return quotientAndMod(other[0]);

    // Just to get short names
    size_type m = _digitsNumber - other._digitsNumber;
    size_type n = other._digitsNumber;
    const Digit base = _base;

    BigUnsignedIntBase10 quotient(m + 1);
    BigUnsignedIntBase10 reminder(n);

    // Normalization
    Digit d = base / (*other.startIter() + 1);
    BigUnsignedIntBase10 u = *this * BigUnsignedIntBase10(std::to_string(d));
    BigUnsignedIntBase10 v = other * BigUnsignedIntBase10(std::to_string(d));

    // Won't work if other has only one digit
    auto condition = [v = v[n - 2], base](Digit q, Digit r, Digit u) {
        return q == base || q * v > base * r + u;
    };

    for (size_type j = m; j != numeric_limits<size_type>::max(); --j)
    {
        Digit estimQuotient = u[j + n] * base + u[j + n - 1];
        Digit estimReminder = estimQuotient % v[n - 1];
        estimQuotient /= v[n - 1];

        if (condition(estimQuotient, estimReminder, u[j + n - 2]))
        {
            --estimQuotient;
            estimReminder += v[n - 1];
            if (condition(estimQuotient, estimReminder, u[j + n - 2]))
                --estimQuotient;
        }

        BigUnsignedIntBase10 uPart(u._digits.begin() + j, u._digits.begin() + j + n + 1);
        BigUnsignedIntBase10 q(std::to_string(estimQuotient));
        BigUnsignedIntBase10 qv = q * v;
        if (uPart >= qv)
            uPart -= qv;
        else
        {
            --estimQuotient;
            uPart -= qv - v;
        }
        copy(uPart._digits.begin(), uPart._digits.end(), u._digits.begin() + j);

        quotient[j] = estimQuotient;
    }

    if (quotient[quotient._digitsNumber - 1] == 0 && quotient._digitsNumber > 1)
        --quotient._digitsNumber;

    u._digitsNumber = n;
    return make_pair(quotient, u.quotientAndMod(d).first);
}

BigUnsignedIntBase10 BigUnsignedIntBase10::pow(const BigUnsignedIntBase10& degree)
{
    // TODO: implement
    return *this;
}

BigUnsignedIntBase10 BigUnsignedIntBase10::multInverse(BigUnsignedIntBase10 v)
{
    BigUnsignedIntBase10 inv, t1, t3, q;
    int iter;
    /* Step X1. Initialise */
    BigUnsignedIntBase10 u1("1");
    BigUnsignedIntBase10 u3(*this);
    BigUnsignedIntBase10 v1("0");
    BigUnsignedIntBase10 v3(v);
    /* Remember odd/even iterations */
    iter = 1;
    /* Step X2. Loop while v3 != 0 */
    while (v3 != BigUnsignedIntBase10("0"))
    {
        /* Step X3. Divide and "Subtract" */
        auto res = u3.quotientAndMod(v3);
        q = res.first;
        t3 = res.second;
        t1 = u1 + q * v1;
        /* Swap */
        u1 = v1;
        v1 = t1;
        u3 = v3;
        v3 = t3;
        iter = -iter;
    }
    /* Make sure u3 = gcd(u,v) == 1 */
    if (u3 != BigUnsignedIntBase10("1"))
        return BigUnsignedIntBase10("0");   /* Error: No inverse exists */
    /* Ensure a positive result */
    if (iter < 0)
        inv = v - u1;
    else
        inv = u1;
    return inv;
}

BigUnsignedIntBase10::size_type BigUnsignedIntBase10::countSignificantNumbers()
{
    size_type zeroesNumber = 0;
    size_type numberOfDigits = 0;
    for (auto it = _digits.begin(); it != _digits.begin() + _digitsNumber; ++it)
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

std::pair<BigUnsignedIntBase10, BigUnsignedIntBase10> BigUnsignedIntBase10::quotientAndMod(Digit d) const
{
    Digit r = 0; // Reminder
    BigUnsignedIntBase10 quotient(_digitsNumber);
    for (size_type j = _digitsNumber - 1; j != numeric_limits<size_type>::max(); --j)
    {
        Digit sum = r * _base + _digits[j];
        r = sum % d;
        quotient[j] = sum / d;
    }

    quotient._digitsNumber = quotient.countSignificantNumbers();
    return std::make_pair(quotient, BigUnsignedIntBase10(std::to_string(r)));
}

BigUnsignedIntBase10::BigUnsignedIntBase10(std::string_view str) : BigUnsignedIntBase10()
{
    createFromString(str, *this);
}

BigUnsignedIntBase10::BigUnsignedIntBase10(UnsignedVector::const_iterator b, UnsignedVector::const_iterator e)
{
    _digits = UnsignedVector(b, e);
    _digitsNumber = size();
}

std::string BigUnsignedIntBase10::to_string() const
{
    stringstream ss;
    for (auto it = _digits.rbegin() + startIndex(); it != _digits.rend(); ++it)
        ss << *it;
    return ss.str();
}

void BigUnsignedIntBase10::resetToZero()
{
    for (Digit& digit:_digits)
        digit = 0;
    _digitsNumber = 1;
}

void BigUnsignedIntBase10::createFromString(string_view str, BigUnsignedIntBase10& number)
{
    using namespace std;

    // Check input
    regex unsignedNumber("[[:digit:]]+");
    if (!regex_match(str.data(), unsignedNumber))
        throw invalid_argument("provided input isn't unsigned integer");

    number._digits = UnsignedVector(str.size());
    number._digitsNumber = str.size();
    // Fill dividend with digits of provided number
    transform(str.rbegin(), str.rend(), number._digits.begin(), [](string::value_type letter) {
        return static_cast<Digit>(letter - '0');
    });
}

void BigUnsignedIntBase10::resize(size_type size) { _digits.resize(size, 0); }