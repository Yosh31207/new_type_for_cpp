// -----------------------------------
// Copyright (c) 2024 Yosh31207
// Distributed under the MIT license.
// -----------------------------------

#ifndef NEW_TYPE_FOR_CPP_HPP_
#define NEW_TYPE_FOR_CPP_HPP_

#include <utility>

namespace newtype {

template<typename T, typename Tag>
class NewType {
private:
    T value_;

public:
    constexpr NewType() : value_() {}
    explicit constexpr NewType(const T& x) : value_(x) {}
    explicit constexpr NewType(T&& x) : value_(std::move(x)) {}

    ~NewType() = default;
    constexpr NewType(const NewType&) = default;
    constexpr NewType(NewType&&) = default;
    constexpr NewType& operator=(const NewType&) = default;
    constexpr NewType& operator=(NewType&&) = default;

    constexpr T& get() {
        return value_;
    };

    constexpr const T& get() const {
        return value_;
    };

    constexpr T& operator*() {
        return value_;
    };

    constexpr const T& operator*() const {
        return value_;
    };

    constexpr T* operator->() {
        return &value_;
    }

    constexpr const T* operator->() const {
        return &value_;
    }

    explicit constexpr operator T() const {
        return value_;
    }

    NewType& operator++() {
        value_++;
        return *this;
    }

    NewType operator++(int) {
        auto temp = *this;
        ++*this;
        return temp;
    }

    NewType& operator--() {
        value_--;
        return *this;
    }

    NewType operator--(int) {
        auto old = *this;
        --*this;
        return old;
    }
};

#define DEFINE_COMPARISON_OPERATOR(op)                                                    \
    template<typename T, typename Tag>                                                    \
    constexpr bool operator op (const NewType<T, Tag>& lhs, const NewType<T, Tag>& rhs) { \
        return lhs.get() op rhs.get();                                                    \
    }

DEFINE_COMPARISON_OPERATOR(<)
DEFINE_COMPARISON_OPERATOR(<=)
DEFINE_COMPARISON_OPERATOR(>)
DEFINE_COMPARISON_OPERATOR(>=)
DEFINE_COMPARISON_OPERATOR(==)
DEFINE_COMPARISON_OPERATOR(!=)
#undef MAKE_COMPARISON_OPERATOR

}  // namespace newtype

#endif  // NEW_TYPE_FOR_CPP_HPP_
