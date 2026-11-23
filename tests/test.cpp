#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <utility>
#include "fraction.h"
#include "fraction_set.h"

// Fraction

TEST(FractionTest, DefaultConstructor) {
    Fraction f;
    EXPECT_EQ(f.numerator(), 0);
    EXPECT_EQ(f.denominator(), 1);
}

TEST(FractionTest, DivisionByZeroThrows) {
    EXPECT_THROW(Fraction(1, 0), std::invalid_argument);
    EXPECT_THROW(Fraction(-5, 0), std::invalid_argument);
    EXPECT_THROW(Fraction(0, 0), std::invalid_argument);
}

TEST(FractionTest, SimplificationGCD) {
    Fraction f1(4, 8);
    EXPECT_EQ(f1.numerator(), 1);
    EXPECT_EQ(f1.denominator(), 2);

    Fraction f2(15, 25);
    EXPECT_EQ(f2.numerator(), 3);
    EXPECT_EQ(f2.denominator(), 5);
}

TEST(FractionTest, SignNormalization) {
    Fraction f1(1, -2);
    EXPECT_EQ(f1.numerator(), -1);
    EXPECT_EQ(f1.denominator(), 2);

    Fraction f2(-2, -4);
    EXPECT_EQ(f2.numerator(), 1);
    EXPECT_EQ(f2.denominator(), 2);
}

TEST(FractionTest, ZeroNumeratorNormalization) {
    Fraction f1(0, 5);
    EXPECT_EQ(f1.numerator(), 0);
    EXPECT_EQ(f1.denominator(), 1);

    Fraction f2(0, -10);
    EXPECT_EQ(f2.numerator(), 0);
    EXPECT_EQ(f2.denominator(), 1);
}

TEST(FractionTest, EqualityOperators) {
    Fraction f1(2, 4);
    Fraction f2(1, 2);
    Fraction f3(-1, 2);

    EXPECT_EQ(f1, f2);
    EXPECT_NE(f1, f3);
}

TEST(FractionTest, StreamOutput) {
    std::ostringstream oss;
    Fraction f(3, 4);
    oss << f;
    EXPECT_EQ(oss.str(), "3/4");
}

// FractionSet

TEST(FractionSetTest, DefaultConstructorIsEmpty) {
    FractionSet s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0);
}

TEST(FractionSetTest, AddAndDuplicates) {
    FractionSet s;
    EXPECT_TRUE(s.add(Fraction(1, 2)));
    EXPECT_EQ(s.size(), 1);

    EXPECT_FALSE(s.add(Fraction(2, 4)));
    EXPECT_EQ(s.size(), 1);

    EXPECT_TRUE(s.add(Fraction(3, 4)));
    EXPECT_EQ(s.size(), 2);
}

TEST(FractionSetTest, ContainsAndCallOperatorConsistency) {
    FractionSet s;
    Fraction f1(1, 3);
    Fraction f2(2, 3);

    s.add(f1);

    EXPECT_TRUE(s.contains(f1));
    EXPECT_TRUE(s(f1)); // operator()

    EXPECT_FALSE(s.contains(f2));
    EXPECT_FALSE(s(f2));
}

TEST(FractionSetTest, RemoveElement) {
    FractionSet s;
    s.add(Fraction(1, 1));
    s.add(Fraction(1, 2));
    s.add(Fraction(1, 3));

    EXPECT_TRUE(s.remove(Fraction(1, 2)));
    EXPECT_EQ(s.size(), 2);
    EXPECT_FALSE(s.contains(Fraction(1, 2)));
    EXPECT_TRUE(s.contains(Fraction(1, 1)));
    EXPECT_TRUE(s.contains(Fraction(1, 3)));

    EXPECT_FALSE(s.remove(Fraction(1, 2)));
    EXPECT_EQ(s.size(), 2);
}

TEST(FractionSetTest, CompoundAssignmentOperators) {
    FractionSet s;
    s += Fraction(1, 2);
    EXPECT_TRUE(s.contains(Fraction(1, 2)));

    s -= Fraction(1, 2);
    EXPECT_FALSE(s.contains(Fraction(1, 2)));
}

TEST(FractionSetTest, CopyConstructorDeep) {
    FractionSet s1;
    s1.add(Fraction(1, 2));
    s1.add(Fraction(3, 4));

    FractionSet s2 = s1;
    EXPECT_EQ(s1.size(), s2.size());
    EXPECT_EQ(s1, s2);

    s2.add(Fraction(5, 6));
    EXPECT_NE(s1.size(), s2.size());
    EXPECT_FALSE(s1.contains(Fraction(5, 6)));
}

TEST(FractionSetTest, CopyAssignmentAndSelfAssignment) {
    FractionSet s1;
    s1.add(Fraction(1, 2));

    FractionSet s2;
    s2 = s1;
    EXPECT_EQ(s1, s2);

    s2 = s2;
    EXPECT_EQ(s2.size(), 1);
    EXPECT_TRUE(s2.contains(Fraction(1, 2)));
}

TEST(FractionSetTest, MoveConstructor) {
    FractionSet s1;
    s1.add(Fraction(1, 2));
    s1.add(Fraction(2, 3));

    FractionSet s2 = std::move(s1);
    EXPECT_EQ(s2.size(), 2);
    EXPECT_TRUE(s2.contains(Fraction(1, 2)));
    EXPECT_TRUE(s2.contains(Fraction(2, 3)));

    EXPECT_EQ(s1.size(), 0);
    EXPECT_TRUE(s1.empty());
}

TEST(FractionSetTest, MoveAssignment) {
    FractionSet s1;
    s1.add(Fraction(1, 4));

    FractionSet s2;
    s2.add(Fraction(3, 4));

    s2 = std::move(s1);
    EXPECT_EQ(s2.size(), 1);
    EXPECT_TRUE(s2.contains(Fraction(1, 4)));
    EXPECT_FALSE(s2.contains(Fraction(3, 4)));

    EXPECT_EQ(s1.size(), 0);
}

TEST(FractionSetTest, UnionOperation) {
    FractionSet a;
    a.add(Fraction(1, 2));
    a.add(Fraction(1, 3));

    FractionSet b;
    b.add(Fraction(1, 3));
    b.add(Fraction(1, 4));

    FractionSet res = a | b;
    EXPECT_EQ(res.size(), 3);
    EXPECT_TRUE(res.contains(Fraction(1, 2)));
    EXPECT_TRUE(res.contains(Fraction(1, 3)));
    EXPECT_TRUE(res.contains(Fraction(1, 4)));
}

TEST(FractionSetTest, IntersectionOperation) {
    FractionSet a;
    a.add(Fraction(1, 2));
    a.add(Fraction(1, 3));

    FractionSet b;
    b.add(Fraction(1, 3));
    b.add(Fraction(1, 4));

    FractionSet res = a & b;
    EXPECT_EQ(res.size(), 1);
    EXPECT_TRUE(res.contains(Fraction(1, 3)));
}

TEST(FractionSetTest, DifferenceOperation) {
    FractionSet a;
    a.add(Fraction(1, 2));
    a.add(Fraction(1, 3));

    FractionSet b;
    b.add(Fraction(1, 3));
    b.add(Fraction(1, 4));

    FractionSet res = a - b;
    EXPECT_EQ(res.size(), 1);
    EXPECT_TRUE(res.contains(Fraction(1, 2)));
}

TEST(FractionSetTest, EqualityAndInequality) {
    FractionSet a;
    a.add(Fraction(1, 2));
    a.add(Fraction(1, 3));

    FractionSet b;
    b.add(Fraction(1, 3));
    b.add(Fraction(2, 4));

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    b.add(Fraction(1, 5));
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(FractionSetTest, StreamOutput) {
    FractionSet s;
    std::ostringstream oss_empty;
    oss_empty << s;
    EXPECT_EQ(oss_empty.str(), "{}");

    s.add(Fraction(1, 2));
    std::ostringstream oss_single;
    oss_single << s;
    EXPECT_EQ(oss_single.str(), "{1/2}");
}