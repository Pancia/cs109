#include <cstdlib>

#include <algorithm>
#include <exception>
#include <limits>
#include <stack>
#include <stdexcept>

#include "bigint.h"
#include "debug.h"

bigint::bigint(long that) {
    bigint(std::to_string(that));
}

bigint::bigint(const std::string& that) {
    this->negative = false;
    std::string reversed = that;
    std::reverse(reversed.begin(), reversed.end());
    for (char val : reversed) {
        if (val == '_') {
            this->negative = true;
        } else {
            this->big_value.push_back(val);
        }
    }
}

bigint::operator  std::string() const {
    std::string ret;
    if (this->negative) {
        ret += "-";
    }
    bigint::bigvalue_t reversed = this->big_value;
    std::reverse(reversed.begin(), reversed.end());
    for (char val : reversed) {
        ret += val;
    }
    return ret;
}

std::string bigint::to_string(bigint::bigvalue_t vec) {
    std::string ret;
    bigint::bigvalue_t reversed = vec;
    std::reverse(reversed.begin(), reversed.end());
    for (char val : reversed) {
        ret += val;
    }
    return ret;
}

bigint operator +(const bigint& left, const bigint& right) {
    DEBUGF('b', "LEFT:" << left << "RIGHT:" << right);
    bigint sum;
    if (left.negative == right.negative) {
        sum.big_value = bigint::do_bigadd
            (left.big_value, right.big_value);
        sum.negative = left.negative;
    } else {
        if (bigint::do_bigless(left.big_value, right.big_value)) {
            sum.big_value = bigint::do_bigsub
                (left.big_value, right.big_value);
            sum.negative = left.negative;
        } else {
            sum.big_value = bigint::do_bigsub
                (right.big_value, left.big_value);
            sum.negative = right.negative;
        }
    }
    return sum;
}

bigint operator -(const bigint& left, const bigint& right) {
    DEBUGF('b', "LEFT:" << left << "RIGHT:" << right);
    bigint::bigint sub;
    if (left.negative == right.negative) {
        sub.big_value = bigint::do_bigsub
            (left.big_value, right.big_value);
        sub.negative = left.negative;
    } else {
        if (bigint::do_bigless(left.big_value, right.big_value)) {
            sub.big_value = bigint::do_bigadd
                (left.big_value, right.big_value);
            sub.negative = left.negative;
        } else {
            sub.big_value = bigint::do_bigadd
                (right.big_value, left.big_value);
            sub.negative = right.negative;
        }
    }
    return sub;
}

bigint operator +(const bigint& right) {
    DEBUGF('b', "RIGHT:" << right);
    throw std::runtime_error("+1 STUB");
}

bigint operator -(const bigint& right) {
    DEBUGF('b', "RIGHT:" << right);
    throw std::runtime_error("-1 STUB");
}

bigint::bigvalue_t bigint::do_bigadd(
        const bigvalue_t& left,
        const bigvalue_t& right) {
    bigint::bigvalue_t padded_left = left;
    bigint::bigvalue_t padded_right = right;
    if (left.size() < right.size()) {
        padded_left.resize(right.size(), '0');
    } else if (right.size() < left.size()) {
        padded_right.resize(left.size(), '0');
    }
    bigvalue_t sum; int carry {0};
    for (size_t i = 0; i < padded_left.size(); i++) {
        char l = padded_left[i];
        char r = padded_right[i];
        char res = l + r - '0' + carry;
        carry = 0;
        if (res - '0' > 9) {
            res -= 10;
            carry = 1;
        }
        sum.push_back(res);
    }
    return bigint::trim_zeros(sum);
}

bigint::bigvalue_t bigint::do_bigsub(
        const bigvalue_t& left,
        const bigvalue_t& right) {
    bigint::bigvalue_t padded_left = left;
    bigint::bigvalue_t padded_right = right;
    if (left.size() < right.size()) {
        padded_left.resize(right.size(), '0');
    } else if (right.size() < left.size()) {
        padded_right.resize(left.size(), '0');
    }
    bigint::bigvalue_t sub;
    int borrow {0};
    for (size_t i = 0; i < padded_left.size(); i++) {
        int l = padded_left[i] - '0';
        int r = padded_right[i] - '0';
        char res = l - r + '0' - borrow;
        if (res - '0' < 0) {
            res += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        sub.push_back(res);
    }
    return bigint::trim_zeros(sub);
}

bigint::bigvalue_t bigint::trim_zeros(bigint::bigvalue_t val) {
    bigint::bigvalue_t reversed = val;
    std::reverse(reversed.begin(), reversed.end());
    for (char c : reversed) {
        if (c == '0')
            val.pop_back();
        else break;
    }
    return val;
}

long bigint::to_long() const {
    DEBUGF('b', "bigint::to_long()");
    throw std::runtime_error("to_long STUB");
    //if (*this <= bigint(std::numeric_limits<long>::min())
    //        or *this > bigint(std::numeric_limits<long>::max()))
    //    throw std::range_error("bigint__to_long: out of range");
    //return long_value;
}

//
// Returns true if left is bigger (or eq?) than right
//
bool bigint::do_bigless(
        const bigvalue_t& left,
        const bigvalue_t& right) {
    if (left.size() < right.size())
        return false;
    if (left.size() > right.size())
        return true;
    for (size_t i = right.size(); i > 0; i--) {
        if (left.at(i-1) < right.at(i-1))
            return false;
        if (left.at(i-1) > right.at(i-1))
            return true;
    }

    return true;
}

bool abs_less(const long& left, const long& right) {
    return left < right;
}

//
// Multiplication algorithm.
//
bigint operator *(const bigint& left, const bigint& right) {
    DEBUGF('b', "LEFT:" << left << "RIGHT:" << right);
    throw std::runtime_error("*2 STUB");
}

//
// Division algorithm.
//
void multiply_by_2(bigint::unumber& unumber_value) {
    unumber_value *= 2;
}

void divide_by_2(bigint::unumber& unumber_value) {
    unumber_value /= 2;
}

bigint::quot_rem divide(const bigint& left, const bigint& right) {
    DEBUGF('b', "LEFT:" << left << "RIGHT:" << right);
    throw std::runtime_error("/2 STUB");
    //if (right == 0)
    //    throw std::domain_error("divide by 0");
    //using unumber = bigint::unumber;
    //static unumber zero = 0;
    //if (right == 0)
    //    throw std::domain_error("bigint::divide");
    //unumber divisor = right;
    //unumber quotient = 0;
    //unumber remainder = left;
    //unumber power_of_2 = 1;
    //while (abs_less(divisor, remainder)) {
    //    multiply_by_2(divisor);
    //    multiply_by_2(power_of_2);
    //}
    //while (abs_less(zero, power_of_2)) {
    //    if (not abs_less(remainder, divisor)) {
    //        remainder = remainder - divisor;
    //        quotient = quotient + power_of_2;
    //    }
    //    divide_by_2(divisor);
    //    divide_by_2(power_of_2);
    //}
    //return {quotient, remainder};
}

bigint operator /(const bigint& left, const bigint& right) {
    return divide(left, right).first;
}

bigint operator %(const bigint& left, const bigint& right) {
    return divide(left, right).second;
}

bool operator ==(const bigint& left, const bigint& right) {
    DEBUGF('b', "LEFT:" << left << "RIGHT:" << right);
    return (left.negative == right.negative)
        && (left.big_value == right.big_value);
}

bool operator <(const bigint& left, const bigint& right) {
    DEBUGF('b', "LEFT:" << left << "RIGHT:" << right);
    if (left.negative != right.negative) {
        return left.negative;
    } else {
        return !bigint::do_bigless(left.big_value, right.big_value);
    }
}

std::ostream& operator <<(std::ostream& out, const bigint& that) {
    out << std::string(that);
    return out;
}

bigint pow(const bigint& base, const bigint& exponent) {
    DEBUGF('^', "base = " << base << ", exponent = " << exponent);
    if (base == 0)
        return 0;
    bigint base_copy = base;
    long expt = exponent.to_long();
    bigint result = 1;
    if (expt < 0) {
        base_copy = 1 / base_copy;
        expt = - expt;
    }
    while (expt > 0) {
        if (expt & 1) { //odd
            result = result * base_copy;
            --expt;
        } else { //even
            base_copy = base_copy * base_copy;
            expt /= 2;
        }
    }
    DEBUGF('^', "result = " << result);
    return result;
}
