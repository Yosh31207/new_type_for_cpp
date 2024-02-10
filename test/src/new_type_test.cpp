// -----------------------------------
// Copyright (c) 2024 Yosh31207
// Distributed under the MIT license.
// -----------------------------------

#include <new_type/new_type.hpp>

#include <string>
#include <type_traits>

#include <gtest/gtest.h>

using newtype::NewType;

namespace {

using MyInt = NewType<int, struct MyIntTag>;
using FirstName = NewType<std::string, struct FirstNameTag>;
using LastName = NewType<std::string, struct LastNameTag>;
using FullName = NewType<std::string, struct FullNameTag>;

FullName make_full_name(const FirstName& first_name, const LastName& last_name) {
    return FullName{*first_name + " " + *last_name};
}

TEST(NewType, Usage) {
    EXPECT_EQ(make_full_name(FirstName("John"), LastName("Cage")).get(), "John Cage");

    // member access
    EXPECT_EQ(FirstName{"John"}->size(), size_t{4});

    // get value
    EXPECT_EQ(FirstName("John").get(), "John");
    EXPECT_EQ(*FirstName("John"), "John");
}

TEST(NewType, Comparison) {
    static_assert(MyInt{1} == MyInt{1});
    static_assert(MyInt{1} != MyInt{2});
    static_assert(MyInt{1} <  MyInt{2});
    static_assert(MyInt{1} <= MyInt{1});
    static_assert(MyInt{1} <= MyInt{2});
    static_assert(MyInt{2} >  MyInt{1});
    static_assert(MyInt{2} >= MyInt{1});
    static_assert(MyInt{2} >= MyInt{2});

    static_assert(!(MyInt{1} == MyInt{2}));
    static_assert(!(MyInt{1} != MyInt{1}));
    static_assert(!(MyInt{2} <  MyInt{1}));
    static_assert(!(MyInt{2} <= MyInt{1}));
    static_assert(!(MyInt{1} >  MyInt{2}));
    static_assert(!(MyInt{1} >= MyInt{2}));
}

TEST(NewType, IncrementAndDecrement) {
    auto value = MyInt{3};
    EXPECT_EQ(++value, MyInt{4});
    value++;
    EXPECT_EQ(value, MyInt{5});
    EXPECT_EQ(--value, MyInt{4});
    value--;
    EXPECT_EQ(value, MyInt{3});
}

TEST(NewType, ExplicitConversion) {
    static_assert(int(MyInt{1}) == 1);
}

TEST(NewType, TypeCheck) {
    // make_full_name(FirstName("John"), LastName("Cage"));       // OK
    // make_full_name(std::string("John"), std::string("Cage"));  // compile-error
    // make_full_name(LastName("Cage"), FirstName("John"));       // compile-error
    static_assert(std::is_invocable_v<decltype(make_full_name), FirstName, LastName>);
    static_assert(!std::is_invocable_v<decltype(make_full_name), LastName, FirstName>);
    static_assert(!std::is_invocable_v<decltype(make_full_name), std::string, std::string>);

    // LastName name = LastName("Cage");     // OK
    // LastName name = FirstName("John");    // compile-error
    // LastName name = std::string("John");  // compile-error
    // std::string name = LastName("John");  // compile-error
    static_assert(std::is_convertible_v<LastName, LastName>);
    static_assert(!std::is_convertible_v<LastName, FirstName>);
    static_assert(!std::is_convertible_v<LastName, std::string>);
    static_assert(!std::is_convertible_v<std::string, LastName>);
}

NEWTYPE_DEFINE_NEW_TYPE(Length, int)
NEWTYPE_DEFINE_NEW_TYPE(Area, int)

constexpr Area operator* (Length x, Length y) {
    return Area{x.get() * y.get()};
}

TEST(NewType, DefiningWithMacro) {
    static_assert(Length{3}.get() == 3);
    static_assert(Length{3} * Length{5} == Area{15});
}

}  // namespace
