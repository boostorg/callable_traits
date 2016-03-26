<!--
Copyright Barrett Adair 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
-->

# CallableTraits

![Build Status](https://travis-ci.org/badair/callable_traits.svg?branch=master)

[![Gitter](https://badges.gitter.im/badair/callable_traits.svg)](https://gitter.im/badair/callable_traits?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

<!--</a> <a target="_blank" href="http://melpon.org/wandbox/permlink/TlioDiz6yYNxZFnv">![Try it online][badge.wandbox]</a>-->

CallableTraits is a small header-only library providing a uniform and comprehensive interface for the type-level manipulation of all callable types in C++. CallableTraits does all the dirty work for you (see an `std::is_function` [implementation ](http://en.cppreference.com/w/cpp/types/is_function#Possible_implementation)  for a glimpse of the "dirty work").

This project is nearing completion. Lack of documentation and spotty code quality are the most glaring issues right now, but progress is being made on both fronts. Test coverage is not 100% yet, but we'll be there soon.

## Overview

<!-- Important: keep this in sync with example/intro.cpp -->
```cpp

#include <type_traits>
#include <functional>
#include <tuple>
#include <callable_traits/callable_traits.hpp>

// Most of this example uses a function object. Unless otherwise noted, all
// features shown in this example can be used for any "callable" types:
// lambdas, generic lambdas, function pointers, function references,
// function types, abominable function types, member function pointers, and
// member data pointers. Ambiguous callables (e.g. function objects with
// templated/overloaded operator()) are not addressed in this example, but
// are recognized and handled by CallableTraits.

// Note: For more information about abominable function types, see Alisdair Meredith's
// proposal at http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0172r0.html

struct foo {
    void operator()(int, int&&, const int&, void* = nullptr) const {}
};

namespace ct = callable_traits;
using namespace std::placeholders;

int main() {

    // indexed argument types
    using second_arg = ct::arg_at<1, foo>;
    static_assert(std::is_same<second_arg, int&&>{}, "");

    // arg types are packaged into std::tuple, which serves as the default
    // type list in CallableTraits (runtime capabilities are not used).
    using args = ct::args<foo>;
    using expected_args = std::tuple<int, int&&, const int&, void*>;
    static_assert(std::is_same<args, expected_args>{}, "");

    //callable_traits::result_of is a bit friendlier than std::result_of 
    using return_type = ct::result_of<foo>;
    static_assert(std::is_same<return_type, void>{}, "");

    // callable_traits::signature yields a plain function type
    using signature = ct::signature<foo>;
    using expected_signature = void(int, int&&, const int&, void*);
    static_assert(std::is_same<signature, expected_signature>{}, "");

    // when trait information can be conveyed in an integral_constant,
    // callable_traits uses constexpr functions instead of template aliases.
    // Note: min_arity and max_arity only differ logically from arity when
    // the argument is a function object.
    static_assert(ct::arity(foo{}) == 4, "");
    static_assert(ct::max_arity(foo{}) == 4, "");
    static_assert(ct::min_arity(foo{}) == 3, "");

    // CallableTraits provides constexpr functions so that the user doesn't
    // need to worry about reference collapsing or decltype when dealing with
    // universal references to callables. Still, you don't need an instance,
    // because CallableTraits provides non-deduced function templates for
    // all constexpr functions besides can_invoke and bind_expr, which
    // model std::invoke and std::bind, respectively (more on these below).
    // Here's an example of the non-deduced functions, which take an explicit
    // type argument. We'll ignore these for the rest of the example.
    static_assert(ct::arity<foo>() == 4, "");
    static_assert(ct::max_arity<foo>() == 4, "");
    static_assert(ct::min_arity<foo>() == 3, "");

    // C-style varargs (ellipses in a signature) can be detected.
    static_assert(!ct::has_varargs(foo{}), "");

    // callable_traits::is_ambiguous yields std::true_type 
    // only when the callable is overloaded or templated.
    static_assert(!ct::is_ambiguous(foo{}), "");

    // callable_traits::can_invoke allows us to preview whether
    // std::invoke will compile with the given arguments. Keep
    // in mind that failing cases must be SFINAE-friendly (i.e.
    // any failing static_asserts can still be tripped). Note: The
    // same sfinae restrictions apply to min_arity and max_arity
    // for function objects.
    int i = 0;
    static_assert(ct::can_invoke(foo{}, 0, 0, i), "");
    // no error:     std::invoke(foo{}, 0, 0, i);

    static_assert(!ct::can_invoke(foo{}, nullptr), "");
    // error:         std::invoke(foo{}, nullptr);

    // callable_traits::bind_expr is a compile-time bind expression parser,
    // very loosely based on the Boost.Bind implementation. Nested bind
    // expressions are fully supported. The return type of bind_expr only
    // contains type information, but can still be used in an evaluated
    // context. The return type can be treated like a callable type when passed
    // to result_of, signature, args, or arg_at template aliases. 
    using bind_expression = decltype(ct::bind_expr(foo{}, _1, _1, _1));

    // Unfortunately, we can't do type manipulations with std::bind directly,
    // because the ISO C++ standard says very little about the return type of
    // std::bind. The purpose of callable_traits::bind_expr is to undo some of
    // the arbitrary black-boxing that std::bind incurs.
    auto bind_obj = std::bind(foo{}, _1, _1, _1);

    // Here, int is chosen as the expected argument for the bind expression
    // because it's the best fit for all three placeholder slots. Behind
    // the scenes, this is determined by a cartesian product of conversion
    // combinations of the known parameter types represented by the reused
    // placeholders (yes, this is slow). The type with the highest number of
    // successful conversions "wins". int is chosen over int&& because
    // non-reference types are preferred in the case of a tie.
    static_assert(std::is_same<
        ct::args<bind_expression>,
        std::tuple<int>
    >{}, "");

    // callable_traits can facilitate the construction of std::function objects.
    auto fn = std::function<ct::signature<bind_expression>>{ bind_obj };
    fn(0);

    // For function objects, the following checks are determined by the 
    // qualifiers on operator(), rather than the category of the passed value.
    // For member function pointers and abominable function types, the
    // qualifier on the function type are used.
    static_assert(ct::is_const_qualified(foo{}), "");
    static_assert(!ct::is_volatile_qualified(foo{}), "");
    static_assert(!ct::is_reference_qualified(foo{}), "");
    static_assert(!ct::is_lvalue_reference_qualified(foo{}), "");
    static_assert(!ct::is_rvalue_reference_qualified(foo{}), "");



    // If you find yourself in the unfortunate situation of needing
    // to manipulate member function pointer types, CallableTraits
    // has all the tools you need to maintain your sanity.



    using pmf = decltype(&foo::operator());

    {
        // So that you don't have to scroll back up to see, here's the type of pmf:
        using expected_pmf = void (foo::*)(int, int&&, const int&, void*) const;
        static_assert(std::is_same<pmf, expected_pmf>{}, "");
    }

    {
        // Let's remove the const qualifier:
        using mutable_pmf = ct::remove_const_qualifier<pmf>;
        using expected_pmf = void (foo::*)(int, int&&, const int&, void*) /*no const!*/;
        static_assert(std::is_same<mutable_pmf, expected_pmf>{}, "");
    }

    {
        // Now let's add an rvalue qualifier (&&):
        using rvalue_pmf = ct::add_rvalue_qualifier<pmf>;
        using expected_pmf = void (foo::*)(int, int&&, const int&, void*) const &&;
        static_assert(std::is_same<rvalue_pmf, expected_pmf>{}, ""); //        ^^^^
    }

    // You get the picture. CallableTraits lets you add and remove all PMF
    // qualifiers (const, volatile, &, &&, and any combination thereof).
    // These type operations can be performed on abominable function types as well.


    // Somehow, somewhere, there was a sad programmer who wanted to
    // manipulate c-style varargs in a function signature. There's
    // really no way to do this universally without doing all the work
    // that CallableTraits already does, so CallableTraits includes in
    // an add/remove feature for this, too.
    {

        using varargs_pmf = ct::add_varargs<pmf>;
        using expected_pmf = void (foo::*)(int, int&&, const int&, void*, ...) const;
        static_assert(std::is_same<varargs_pmf, expected_pmf>{}, ""); //  ^^^

        // note: MSVC likely requires __cdecl for a varargs PMF on your
        // machine, at least if you intend to do anything useful with it.
    }

    return 0;
}
```
## Compatibility

CallableTraits is currently tested and working on the following platforms, unless otherwise noted:
- Linux
  - clang 3.5 and later (both libc++ and libstdc++)
  - gcc 5.2 and later
- OSX
  - Apple Xcode 6.3 and later
  - open-source clang 3.5 and later should work, but is not tested
- Windows
  - Microsoft Visual Studio 2015 (native MSVC)
  - MinGW32 - GCC 5.3 (other versions not tested)
  - clang-cl in Visual Studio's LLVM toolkit cannot build CallableTraits tests because of [this curious bug](http://stackoverflow.com/questions/36026352/compiler-attribute-stuck-on-a-function-type-is-there-a-workaround-for-this-cla). I filed a bug report, but I should be able to work around it when I find the time to do so.

I do not know the compatibility of CallableTraits for other/older compilers, but the `stdlib` implementation must include `std::index_sequence` and friends.

## Dependencies

CallableTraits does not use Boost or any other libraries outside of the standard headers.

## Building the tests

First, you'll need a recent version of [CMake](https://cmake.org/). These commands assume that `git` and `cmake` are available in your environment path. If you need help with this, [message me on Gitter](https://gitter.im/badair/callable_traits).

__GNU/Linux/OSX__

Open a shell and enter the following commands:

```shell
git clone http://github.com/badair/callable_traits
cd callable_traits
mkdir build
cd build
cmake ..
make check
```
If your system doesn't have a default C++ compiler, or your default C++ compiler is too old, you'll need to point CMake to a compatible C++ compiler like this, before running `make check`:

```shell
cmake .. -DCMAKE_CXX_COMPILER=/path/to/compiler
```

CMake should yell at you if your compiler is too old.

__Windows__

Cygwin/MSYS/MSYS2 users should refer to the Linux section. For Visual Studio 2015, fire up `cmd.exe` and enter the following commands:

```shell
git clone http://github.com/badair/callable_traits
cd callable_traits
mkdir build
cd build
cmake .. -G"Visual Studio 14 2015 Win64"
```
Then, open the generated `callable_traits.sln` solution file in Visual Studio.

## License
Please see [LICENSE.md](LICENSE.md).

<!-- Links -->
[badge.Wandbox]: https://img.shields.io/badge/try%20it-online-blue.svg
[example.Wandbox]: http://melpon.org/wandbox/permlink/TlioDiz6yYNxZFnv

