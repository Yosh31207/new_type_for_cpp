# New Type for C++

This single-header library provides an easy way to define a new type from existing type. This is similar to what is called New Type Idiom in Rust.

## Requirements

* C++ 17 or later

## Installation

Include `new_type.hpp`.

## Motivation

Here is a motivating example:

```C++
int get_user_id(const std::string& user_name);
int get_game_id(const std::string& game_title);
PlayInfo get_play_info(int user_id, int game_id);

int game_id = get_game_id("Legend of Zelda");
int user_id = get_user_id("John");
PlayInfo info = get_play_info(game_id, user_id);
```

Spot the error?

The arguments for `get_play_info` are swapped. Complier reports no error, because both user IDs and game IDs are simply integers, even though the function call is semantically wrong.

We can solve this problem by defining separate types for different kinds of IDs with new types.

```C++
#include <new_type/new_type.hpp>

using UserId = newtype::NewType<int, struct UserIdTag>;
using GameId = newtype::NewType<int, struct GameIdTag>;

UserId get_user_id(const std::string& user_name);
GameId get_game_id(const std::string& game_title);
PlayInfo get_play_info(int user_id, int game_id);

GameId game_id  = get_game_id("Legend of Zelda");
UserId user_id  = get_user_id("John");
PlayInfo info_1 = get_play_info(user_id, game_id);  // OK
PlayInfo info_2 = get_play_info(game_id, user_id);  // compile-error!
```

This does not add any new behavior, but defines non-exchangeable types to specify the domain and intended usage for general data type such as integers.

## Usage

To define a new type from existing type T, declare it as like below.

```C++
#include <new_type/new_type.hpp>

using MyType = newtype::NewType<T, Tag>;
```

The new type needs an identification tag. This tag can be any struct or class type, and it is not necessary to define it. Forward declaration would suffice.

Alternatively, you can define types with the `NEWTYPE_DEFINE_NEW_TYPE` macro.

```C++
NEWTYPE_DEFINE_NEW_TYPE(MyType, T)
```

Here is example:

```C++
// without macro
using UserId    = newtype::NewType<int, struct UserIdTag>;
using GameId    = newtype::NewType<int, struct GameIdTag>;
using FirstName = newtype::NewType<std::string, struct FirstNameTag>;
using LastName  = newtype::NewType<std::string, struct LastNameTag>;

// with macro
NEWTYPE_DEFINE_NEW_TYPE(UserId, int)
NEWTYPE_DEFINE_NEW_TYPE(GameId, int)
NEWTYPE_DEFINE_NEW_TYPE(FirstName, std::string)
NEWTYPE_DEFINE_NEW_TYPE(LastName, std::string)
```

To make an instance, pass a value of original type to the constructor.

```C++
MyInt value = MyInt{1};   // or MyInt(1)
```

To get the containing value, use indirection operator `*` or call `get()`.

```C++
MyInt value = MyInt{1};

assert(*value == 1);
assert(value.get() == 1);
```

To call a method of the containing type, use arrow operator `->``.

```C++
using FirstName = newtype::NewType<std::string, struct FirstNameTag>;

FirstName value = FirstName{"John"};
assert(value->size() == 4);
```

You can compare values of a same new type.

```C++
static_assert(MyInt{1} != MyInt{2});
static_assert(MyInt{1} <  MyInt{2});
```

A new type can not be converted to another new type, even if the containing types are same.

```C++
using MyInt1 = newtype::NewType<int, struct MyIntTag1>;
using MyInt2 = newtype::NewType<int, struct MyIntTag2>;

void func(MyInt1 x);  // (A)
void func(MyInt2 x);  // (B)

func(MyInt1{0});   // (A) is called
func(MyInt2{0});   // (B) is called
func(0);           // compile-error

MyInt1 value = MyInt2{0};   // compile-error
```

New types can be used in a constexpr context

```C++
constexpr MyInt x = MyInt{10};
```

## Tested Compiler

* gcc 11.4.0

## License

MIT License
