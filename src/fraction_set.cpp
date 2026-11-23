#include "fraction_set.h"
#include <utility>

FractionSet::FractionSet() : data_(nullptr), size_(0), capacity_(0) {}

FractionSet::~FractionSet() noexcept
{
    delete[] data_;
}

FractionSet::FractionSet(const FractionSet &other) : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ > 0) {
        data_ = new Fraction[capacity_];
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
}

FractionSet::FractionSet(FractionSet &&other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

FractionSet& FractionSet::operator=(const FractionSet& other) {
    if (this != &other) {
        FractionSet tmp(other);
        swap(tmp);
    }

    return *this;
}

FractionSet& FractionSet::operator=(FractionSet&& other) noexcept {
    if (this != &other) {
        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    return *this;
}

void FractionSet::swap(FractionSet& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

void FractionSet::reallocate(std::size_t new_capacity) {
    if (new_capacity == capacity_) {
        return;
    }

    Fraction* new_data = nullptr;
    if (new_capacity > 0) {
        new_data = new Fraction[new_capacity];
    }

    for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
    }

    delete[] data_;

    data_ = new_data;
    capacity_ = new_capacity;
}

bool FractionSet::contains(const Fraction& elem) const {
    for (std::size_t i = 0; i < size_; i ++) {
        if (data_[i] == elem) {
            return true;
        }
    }

    return false;
}

bool FractionSet::add(const Fraction& elem) {
    if (contains(elem)) {
        return false;
    }

    if (size_ == capacity_) {
        std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }

    data_[size_++] = elem;
    return true;
} // true — множество изменилось


bool FractionSet::remove(const Fraction& elem) {
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] == elem) {
            data_[i] = data_[size_ - 1];
            --size_;
            return true;
        }
    }

    return false;
} // true — элемент был и удалён


bool FractionSet::operator()(const Fraction& elem) const {
    return contains(elem);
}

std::size_t FractionSet::size() const {
    return size_;
}

bool FractionSet::empty() const {
    return size() == 0;
}

FractionSet& FractionSet::operator+=(const Fraction& elem) {
    add(elem);
    return *this;
}

FractionSet& FractionSet::operator-=(const Fraction& elem) {
    remove(elem);
    return *this;
}

FractionSet FractionSet::operator|(const FractionSet& other) const {
    FractionSet result(*this);
    
    for (std::size_t i = 0; i < other.size_; ++i) {
        result.add(other.data_[i]);
    }

    return result;
}

FractionSet FractionSet::operator&(const FractionSet& other) const {
    FractionSet result;

    for (std::size_t i = 0; i < size_; ++i) {
        if (other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }

    return result;
}

FractionSet FractionSet::operator-(const FractionSet& other) const {
    FractionSet result;

    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }

    return result;
}

bool FractionSet::operator==(const FractionSet& other) const {
    if (size_ != other.size()) {
        return false;
    }

    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.contains(data_[i])) {
            return false;
        }
    }

    return true;
}

bool FractionSet::operator!=(const FractionSet& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const FractionSet& set) {
    os << "{";
    for (std::size_t i = 0; i < set.size_; ++i) {
        os << set.data_[i];
        if (i + 1 < set.size_) {
            os << ", ";
        }
    }
    os << "}";
    return os;
}