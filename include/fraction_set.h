#pragma once

#include "fraction.h"
#include <iostream>

class FractionSet final {
private:
    Fraction* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;

    void reallocate(std::size_t new_capacity);
public:
    FractionSet();
    FractionSet(const FractionSet&);
    FractionSet(FractionSet&&) noexcept;
    FractionSet& operator=(const FractionSet&);
    FractionSet& operator=(FractionSet&&) noexcept;
    ~FractionSet() noexcept;

    bool add(const Fraction&); // true — множество изменилось
    bool remove(const Fraction&); // true — элемент был и удалён
    bool contains(const Fraction&) const;
    bool operator()(const Fraction&) const; // принадлежность

    std::size_t size() const;
    bool empty() const;
    
    FractionSet& operator+=(const Fraction&);
    FractionSet& operator-=(const Fraction&);
    
    FractionSet operator|(const FractionSet&) const;
    FractionSet operator&(const FractionSet&) const;
    FractionSet operator-(const FractionSet&) const;
    
    bool operator==(const FractionSet&) const;
    bool operator!=(const FractionSet&) const;
    
    friend std::ostream& operator<<(std::ostream&, const FractionSet&);

    void swap(FractionSet& other) noexcept;
};