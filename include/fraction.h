#pragma once

#include <stdexcept>
#include <cmath>
#include <iostream>

class Fraction final {
private:
    long long numerator_;
    long long denominator_;

    static long long gcd(long long a, long long b) {
        a = std::abs(a);
        b = std::abs(b);

        while (b != 0) {
            long long temp = b;
            b = a % b;
            a = temp;
        }
        
        return a;
    }

    void canonicalize() {
        if (denominator_ == 0) {
            throw std::invalid_argument("denuminator cannot be zero");
        }

        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }

        if (numerator_ == 0) {
            denominator_ = 1;
            return;
        }

        long long g = gcd(numerator_, denominator_);
        numerator_ /= g;
        denominator_ /= g;
    }

public:
    Fraction() noexcept : numerator_(0), denominator_(1) {}

    Fraction(long long numerator, long long denominator) : numerator_(numerator), denominator_(denominator) {
        canonicalize();
    }

    Fraction(const Fraction&) noexcept = default;
    Fraction(Fraction&&) noexcept = default;
    Fraction& operator=(const Fraction&) noexcept = default;
    Fraction& operator=(Fraction&&) noexcept = default;
    ~Fraction() noexcept = default;

    long long numerator() const noexcept { return numerator_; }
    long long denominator() const noexcept { return denominator_; }

    bool operator==(const Fraction& other) const noexcept = default;
};

inline std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    return os << f.numerator() << "/" << f.denominator();
}