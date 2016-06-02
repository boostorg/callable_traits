/*<-
Copyright Barrett Adair 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http ://boost.org/LICENSE_1_0.txt)
->*/

//[ push_front
#include <callable_traits/push_front.hpp>

namespace ct = callable_traits;

int main() {

    using f = void(int, char);
    using test = ct::args_push_front_t<f, long, void*>;
    using expect = void(long, void*, int, char);
    static_assert(std::is_same<test, expect>::value, "");
}
//]
