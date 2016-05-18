/*
Copyright (c) 2016 Modified Work Barrett Adair

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

HEADER GUARDS INTENTIONALLY OMITTED
DO NOT INCLUDE THIS HEADER DIRECTLY
*/

template<typename Ret, typename... Args>
struct add_calling_convention_t<
    Ret(*)(Args..., ...), CALLABLE_TRAITS_CC_TAG> {
    using type = CALLABLE_TRAITS_ST Ret(CALLABLE_TRAITS_VARARGS_CC*)(Args..., ...);
};

template<typename Ret, typename... Args>
struct has_calling_convention_t<
    CALLABLE_TRAITS_ST Ret(CALLABLE_TRAITS_VARARGS_CC*)(Args..., ...), CALLABLE_TRAITS_CC_TAG> {
    using type = std::true_type;
};

template<typename Return, typename... Args>
struct function<CALLABLE_TRAITS_ST Return(CALLABLE_TRAITS_VARARGS_CC *)(Args..., ...)>
 : qualifier_traits<dummy>, default_callable_traits {

    static constexpr bool value = true;

    using is_function = std::true_type;
    using has_varargs = std::true_type;
    using traits = function;
    using return_type = Return;
    using arg_types = std::tuple<Args...>;

    using remove_calling_convention =
        Return(CALLABLE_TRAITS_DEFAULT_VARARGS_CC*)(Args..., ...);

    using type =
        CALLABLE_TRAITS_ST Return(CALLABLE_TRAITS_VARARGS_CC *)(Args..., ...);

    using function_type = Return(Args..., ...);
    using qualified_function_type = function_type;

    using remove_varargs =
        CALLABLE_TRAITS_ST Return(CALLABLE_TRAITS_CC *)(Args...);

    using add_varargs = type;
    using remove_member_pointer = type;

    template<typename U>
    using apply_member_pointer =
        CALLABLE_TRAITS_ST Return(CALLABLE_TRAITS_VARARGS_CC U::*)(Args..., ...);

    template<typename NewReturn>
    using apply_return =
        CALLABLE_TRAITS_ST NewReturn(CALLABLE_TRAITS_VARARGS_CC *)(Args..., ...);

    template<template<class...> class Container>
    using expand_args = Container<Args...>;

    using clear_args =
        CALLABLE_TRAITS_ST Return(CALLABLE_TRAITS_VARARGS_CC *)();

#undef CALLABLE_TRAITS_BEGIN_PACK_MANIP
#undef CALLABLE_TRAITS_ARGS_PACK
#undef CALLABLE_TRAITS_END_PACK_MANIP

#define CALLABLE_TRAITS_BEGIN_PACK_MANIP Return(CALLABLE_TRAITS_VARARGS_CC *)(

#define CALLABLE_TRAITS_ARGS_PACK Args

#define CALLABLE_TRAITS_END_PACK_MANIP , ...)

#include <callable_traits/detail/unguarded/args_pack_manipulations.hpp>
#undef CALLABLE_TRAITS_BEGIN_PACK_MANIP
#undef CALLABLE_TRAITS_END_PACK_MANIP
};

