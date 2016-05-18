/*
Copyright Barrett Adair 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http ://boost.org/LICENSE_1_0.txt)
*/

#include <type_traits>
#include <functional>
#include <tuple>
#include <callable_traits/remove_member_const.hpp>

#ifndef CT_ASSERT
#define CT_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif //CT_ASSERT

#ifdef CALLABLE_TRAITS_DISABLE_REFERENCE_QUALIFIERS
#define LREF
#define RREF
#else
#define LREF &
#define RREF &&
#endif

struct foo {};

namespace ct = callable_traits;

int main() {

    {
        using f =   void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...);
        using l =   void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...) LREF;
        using r =   void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...) RREF ;
        using c =   void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...) const;
        using cl =  void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...) const LREF;
        using cr =  void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...) const RREF;
        using v =   void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...) volatile;
        using vl =  void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...) volatile LREF;
        using vr =  void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...) volatile RREF;
        using cv =  void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...) const volatile;
        using cvl = void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...) const volatile LREF;
        using cvr = void(CALLABLE_TRAITS_DEFAULT_VARARGS_CC foo::*)(...) const volatile RREF;

        CT_ASSERT(std::is_same<f,  ct::remove_member_const<f>>{});
        CT_ASSERT(std::is_same<f,  ct::remove_member_const<c>>{});
        CT_ASSERT(std::is_same<l,  ct::remove_member_const<l>>{});
        CT_ASSERT(std::is_same<l,  ct::remove_member_const<cl>>{});
        CT_ASSERT(std::is_same<r,  ct::remove_member_const<r>>{});
        CT_ASSERT(std::is_same<r,  ct::remove_member_const<cr>>{});
        CT_ASSERT(std::is_same<v,  ct::remove_member_const<v>>{});
        CT_ASSERT(std::is_same<v,  ct::remove_member_const<cv>>{});
        CT_ASSERT(std::is_same<vl, ct::remove_member_const<vl>>{});
        CT_ASSERT(std::is_same<vl, ct::remove_member_const<cvl>>{});
        CT_ASSERT(std::is_same<vr, ct::remove_member_const<vr>>{});
        CT_ASSERT(std::is_same<vr, ct::remove_member_const<cvr>>{});
    }

    {
        using f =   foo const & (foo::*)(int, int);
        using l =   foo const & (foo::*)(int, int) LREF;
        using r =   foo const & (foo::*)(int, int) RREF ;
        using c =   foo const & (foo::*)(int, int) const;
        using cl =  foo const & (foo::*)(int, int) const LREF;
        using cr =  foo const & (foo::*)(int, int) const RREF;
        using v =   foo const & (foo::*)(int, int) volatile;
        using vl =  foo const & (foo::*)(int, int) volatile LREF;
        using vr =  foo const & (foo::*)(int, int) volatile RREF;
        using cv =  foo const & (foo::*)(int, int) const volatile;
        using cvl = foo const & (foo::*)(int, int) const volatile LREF;
        using cvr = foo const & (foo::*)(int, int) const volatile RREF;

        CT_ASSERT(std::is_same<f,  ct::remove_member_const<f>>{});
        CT_ASSERT(std::is_same<f,  ct::remove_member_const<c>>{});
        CT_ASSERT(std::is_same<l,  ct::remove_member_const<l>>{});
        CT_ASSERT(std::is_same<l,  ct::remove_member_const<cl>>{});
        CT_ASSERT(std::is_same<r,  ct::remove_member_const<r>>{});
        CT_ASSERT(std::is_same<r,  ct::remove_member_const<cr>>{});
        CT_ASSERT(std::is_same<v,  ct::remove_member_const<v>>{});
        CT_ASSERT(std::is_same<v,  ct::remove_member_const<cv>>{});
        CT_ASSERT(std::is_same<vl, ct::remove_member_const<vl>>{});
        CT_ASSERT(std::is_same<vl, ct::remove_member_const<cvl>>{});
        CT_ASSERT(std::is_same<vr, ct::remove_member_const<vr>>{});
        CT_ASSERT(std::is_same<vr, ct::remove_member_const<cvr>>{});
    }

#ifndef CALLABLE_TRAITS_DISABLE_ABOMINABLE_FUNCTIONS

    {
        using f =   void();
        using l =   void() LREF;
        using r =   void() RREF ;
        using c =   void() const;
        using cl =  void() const LREF;
        using cr =  void() const RREF;
        using v =   void() volatile;
        using vl =  void() volatile LREF;
        using vr =  void() volatile RREF;
        using cv =  void() const volatile;
        using cvl = void() const volatile LREF;
        using cvr = void() const volatile RREF;

        CT_ASSERT(std::is_same<f,  ct::remove_member_const<f>>{});
        CT_ASSERT(std::is_same<f,  ct::remove_member_const<c>>{});
        CT_ASSERT(std::is_same<l,  ct::remove_member_const<l>>{});
        CT_ASSERT(std::is_same<l,  ct::remove_member_const<cl>>{});
        CT_ASSERT(std::is_same<r,  ct::remove_member_const<r>>{});
        CT_ASSERT(std::is_same<r,  ct::remove_member_const<cr>>{});
        CT_ASSERT(std::is_same<v,  ct::remove_member_const<v>>{});
        CT_ASSERT(std::is_same<v,  ct::remove_member_const<cv>>{});
        CT_ASSERT(std::is_same<vl, ct::remove_member_const<vl>>{});
        CT_ASSERT(std::is_same<vl, ct::remove_member_const<cvl>>{});
        CT_ASSERT(std::is_same<vr, ct::remove_member_const<vr>>{});
        CT_ASSERT(std::is_same<vr, ct::remove_member_const<cvr>>{});
    }

#endif
}
