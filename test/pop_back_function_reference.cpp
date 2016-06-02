#include <tuple>
#include <utility>
#include <type_traits>
#include <callable_traits/callable_traits.hpp>

#ifndef CT_ASSERT
#define CT_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif //CT_ASSERT

namespace ct = callable_traits;

template<int I>
struct N {};

template<typename... Ts>
using sig = const char*(&)(Ts...);

int main() {
    {
        using f = sig<N<0>, N<1>, N<2>, N<3>, N<4>>;
        using test = ct::args_pop_back_t<f, 2>;
        using expect = sig<N<0>, N<1>, N<2>>;
        CT_ASSERT(std::is_same<test, expect>::value);
    }

    {
        using f = sig<N<0>, N<1>, N<2>, N<3>, N<4>>;
        using test = ct::args_pop_back_t<f, 5>;
        using expect = sig<>;
        CT_ASSERT(std::is_same<test, expect>::value);
    }

    {
        using f = sig<N<0>, N<1>, N<2>, N<3>, N<4>>;
        using test = ct::args_pop_back_t<f, 0>;
        using expect = sig<N<0>, N<1>, N<2>, N<3>, N<4>>;
        CT_ASSERT(std::is_same<test, expect>::value);
    }

    {
        using f = sig<N<0>, N<1>, N<2>, N<3>, N<4>>;
        using test = ct::args_pop_back_t<f>;
        using expect = sig<N<0>, N<1>, N<2>, N<3>>;
        CT_ASSERT(std::is_same<test, expect>::value);
    }
}

