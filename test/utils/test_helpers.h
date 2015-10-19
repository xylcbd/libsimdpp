/*  Copyright (C) 2012  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef LIBSIMDPP_TEST_UTILS_TEST_HELPERS_H
#define LIBSIMDPP_TEST_UTILS_TEST_HELPERS_H

#include <simdpp/simd.h>
#include <simdpp/detail/align_v128.h>
#include <iostream>
#include "test_suite.h"

namespace SIMDPP_ARCH_NAMESPACE {

/*  A bunch of overloads that wrap the TestSuite::push() method. The push()
    method accepts a type enum plus a pointer; the wrapper overloads determine
    the type enum from the type of the supplied argument.
    @{
*/
inline void test_push_internal(TestSuite& t, std::uint16_t data, unsigned line)
{
    t.push(TestSuite::TYPE_UINT16, 1, line).set(0, &data);
}

template<class V>
void test_push_internal_vec(TestSuite::Result& res, const V& data)
{
    for (unsigned i = 0; i < data.vec_length; ++i) {
        using Base = typename V::base_vector_type;
        simdpp::detail::mem_block<Base> block(data.vec(i));
        for (unsigned j = 0; j < Base::length; j++) {
            res.set(i*Base::length + j, &block[j]);
        }
    }
}

template<unsigned N>
void test_push_internal(TestSuite& t, const simdpp::int8<N>& data, unsigned line)
{
    test_push_internal_vec(t.push(TestSuite::TYPE_INT8, N, line), data);
}

template<unsigned N>
void test_push_internal(TestSuite& t, const simdpp::uint8<N>& data, unsigned line)
{
    test_push_internal_vec(t.push(TestSuite::TYPE_UINT8, N, line), data);
}
template<unsigned N>
void test_push_internal(TestSuite& t, const simdpp::int16<N>& data, unsigned line)
{
    test_push_internal_vec(t.push(TestSuite::TYPE_INT16, N, line), data);
}

template<unsigned N>
void test_push_internal(TestSuite& t, const simdpp::uint16<N>& data, unsigned line)
{
    test_push_internal_vec(t.push(TestSuite::TYPE_UINT16, N, line), data);
}
template<unsigned N>
void test_push_internal(TestSuite& t, const simdpp::int32<N>& data, unsigned line)
{
    test_push_internal_vec(t.push(TestSuite::TYPE_INT32, N, line), data);
}

template<unsigned N>
void test_push_internal(TestSuite& t, const simdpp::uint32<N>& data, unsigned line)
{
    test_push_internal_vec(t.push(TestSuite::TYPE_UINT32, N, line), data);
}
template<unsigned N>
void test_push_internal(TestSuite& t, const simdpp::int64<N>& data, unsigned line)
{
    test_push_internal_vec(t.push(TestSuite::TYPE_INT64, N, line), data);
}

template<unsigned N>
void test_push_internal(TestSuite& t, const simdpp::uint64<N>& data, unsigned line)
{
    test_push_internal_vec(t.push(TestSuite::TYPE_UINT64, N, line), data);
}

template<unsigned N>
void test_push_internal(TestSuite& t, const simdpp::float32<N>& data, unsigned line)
{
    test_push_internal_vec(t.push(TestSuite::TYPE_FLOAT32, N, line), data);
}

template<unsigned N>
void test_push_internal(TestSuite& t, const simdpp::float64<N>& data, unsigned line)
{
    test_push_internal_vec(t.push(TestSuite::TYPE_FLOAT64, N, line), data);
}
// @}
} // namespace SIMDPP_ARCH_NAMESPACE

// we are supposed to call this from within the test function which is in
// SIMDPP_ARCH_NAMESPACE namespace

/*
    T - type
    D - an object of type T to push
    A - array of objects to push
    O1, Q2 - arguments to apply OP to
    OP - operation to apply to the array or arguments
    R - type to cast the object to be pushed to
*/
#define TEST_PUSH(TC,T,D)   { test_push_internal((TC), (T)(D), __LINE__); }

#define NEW_TEST_SUITE(R, NAME) ((R).new_test_suite((NAME), __FILE__))

#define TEST_ARRAY_PUSH(TC, T, A)                                       \
{                                                                       \
    (TC).reset_seq();                                                   \
    for (unsigned i = 0; i < sizeof(A) / sizeof(T); i++) {              \
        const T* lp = reinterpret_cast<const T*>((A) + i);              \
        TEST_PUSH(TC, T, *lp);                                          \
    }                                                                   \
}

#define TEST_ARRAY_HELPER1(TC, T, OP, A)                                \
{                                                                       \
    (TC).reset_seq();                                                   \
    for (unsigned i = 0; i < sizeof(A) / sizeof(T); i++) {              \
        const T* lp = reinterpret_cast<const T*>((A) + i);              \
        TEST_PUSH(TC, T, OP(*lp));                                      \
    }                                                                   \
}

#define TEST_ARRAY_HELPER1_T(TC, R, T, OP, A)                           \
{                                                                       \
    (TC).reset_seq();                                                   \
    for (unsigned i = 0; i < sizeof(A) / sizeof(T); i++) {              \
        const T* lp = reinterpret_cast<const T*>((A) + i);              \
        TEST_PUSH(TC, R, OP(*lp));                                      \
    }                                                                   \
}

#define TEST_ARRAY_HELPER2(TC, T, OP, A, B)                             \
{                                                                       \
    (TC).reset_seq();                                                   \
    for (unsigned i = 0; i < sizeof(A) / sizeof(T); i++) {              \
        const T* lp = reinterpret_cast<const T*>((A) + i);              \
        const T* rp = reinterpret_cast<const T*>((B) + i);              \
        TEST_PUSH(TC, T, OP(*lp, *rp));                                 \
    }                                                                   \
}

// tests OP on all pairs of elements within array A
#define TEST_ALL_COMB_HELPER1(TC, T, OP, A, EL_SIZE)                    \
{                                                                       \
    (TC).reset_seq();                                                   \
    for (unsigned i = 0; i < sizeof(A) / sizeof(T); i++) {              \
        const T* lp = reinterpret_cast<const T*>((A) + i);              \
        T l = *lp;                                                      \
        for (unsigned rot = 0; rot < sizeof(T)/EL_SIZE; rot++) {        \
            TEST_PUSH(TC, T, OP(l));                                    \
            l = simdpp::detail::align_v128<1>(l, l);                    \
        }                                                               \
    }                                                                   \
}

#define TEST_ALL_COMB_HELPER1_T(TC, R, T, OP, A, EL_SIZE)                \
{                                                                       \
    (TC).reset_seq();                                                   \
    for (unsigned i = 0; i < sizeof(A) / sizeof(T); i++) {              \
        const T* lp = reinterpret_cast<const T*>((A) + i);              \
        T l = *lp;                                                      \
        for (unsigned rot = 0; rot < sizeof(T)/EL_SIZE; rot++) {        \
            TEST_PUSH(TC, R, OP(l));                                    \
            l = simdpp::detail::align_v128<1>(l, l);                    \
        }                                                               \
    }                                                                   \
}

#define TEST_ALL_COMB_HELPER2(TC, T, OP, A, EL_SIZE)                    \
{                                                                       \
    (TC).reset_seq();                                                   \
    for (unsigned i = 0; i < sizeof(A) / sizeof(T); i++) {              \
        for (unsigned j = 0; j < sizeof(A) / sizeof(T); j++) {          \
            const T* lp = reinterpret_cast<const T*>((A) + i);          \
            const T* rp = reinterpret_cast<const T*>((A) + j);          \
            T l = *lp; T r = *rp;                                       \
            for (unsigned rot = 0; rot < sizeof(T)/EL_SIZE; rot++) {    \
                TEST_PUSH(TC, T, OP(l, r));                             \
                l = simdpp::detail::align_v128<1>(l, l);                \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#define TEST_ALL_COMB_HELPER2_T(TC, R, T, OP, A, EL_SIZE)               \
{                                                                       \
    (TC).reset_seq();                                                   \
    for (unsigned i = 0; i < sizeof(A) / sizeof(T); i++) {              \
        for (unsigned j = 0; j < sizeof(A) / sizeof(T); j++) {          \
            const T* lp = reinterpret_cast<const T*>((A) + i);          \
            const T* rp = reinterpret_cast<const T*>((A) + j);          \
            T l = *lp; T r = *rp;                                       \
            for (unsigned rot = 0; rot < sizeof(T)/EL_SIZE; rot++) {    \
                TEST_PUSH(TC, R, OP(l, r));                             \
                l = simdpp::detail::align_v128<1>(l, l);                \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#define TEST_ALL_COMB_HELPER3(TC, T, OP, A, EL_SIZE)                    \
{                                                                       \
    (TC).reset_seq();                                                   \
    for (unsigned i0 = 0; i0 < sizeof(A) / sizeof(T); i0++) {           \
    for (unsigned i1 = 0; i1 < sizeof(A) / sizeof(T); i1++) {           \
    for (unsigned i2 = 0; i2 < sizeof(A) / sizeof(T); i2++) {           \
        const T* p0 = reinterpret_cast<const T*>((A) + i0);             \
        const T* p1 = reinterpret_cast<const T*>((A) + i1);             \
        const T* p2 = reinterpret_cast<const T*>((A) + i2);             \
        T v0 = *p0; T v1 = *p1; T v2 = *p2;                             \
        for (unsigned rot0 = 0; rot0 < sizeof(T)/EL_SIZE % 4; rot0++) { \
            for (unsigned rot1 = 0; rot1 < sizeof(T)/EL_SIZE % 4; rot1++) { \
                TEST_PUSH(TC, T, OP(v0, v1, v2));                       \
                v0 = simdpp::detail::align_v128<1>(v0, v0);             \
            }                                                           \
            v1 = simdpp::detail::align_v128<1>(v1, v1);                 \
        }                                                               \
    }}}                                                                 \
}

/**
    Used to test functions that depend on a template parameter. Essentially
    implements a for loop realised in templates.

    The template calls F<V, i>::test(tc, a) for each i from 0 to F<V, 0>::limit.
*/
template<template<class, unsigned> class F, class V, bool large, unsigned i, unsigned limit>
struct TemplateTestHelperImpl;

template<template<class, unsigned> class F, class V, unsigned i, unsigned limit>
struct TemplateTestHelperImpl<F, V, false, i, limit> {

    static void run(TestSuite& tc, const V& a)
    {
        F<V, i>::test(tc, a);
        const bool is_large = i + 30 < limit;
        TemplateTestHelperImpl<F, V, is_large, i+1, limit>::run(tc, a);
    }

    static void run(TestSuite& tc, const V& a, const V& b)
    {
        F<V, i>::test(tc, a, b);
        const bool is_large = i + 30 < limit;
        TemplateTestHelperImpl<F, V, is_large, i+1, limit>::run(tc, a, b);
    }
};

template<template<class, unsigned> class F, class V, unsigned i, unsigned limit>
struct TemplateTestHelperImpl<F, V, true, i, limit> {

    static void run(TestSuite& tc,const V& a)
    {
        F<V, i>::test(tc, a);
        F<V, i+1>::test(tc, a);
        F<V, i+2>::test(tc, a);
        F<V, i+3>::test(tc, a);
        F<V, i+4>::test(tc, a);
        F<V, i+5>::test(tc, a);
        F<V, i+6>::test(tc, a);
        F<V, i+7>::test(tc, a);
        F<V, i+8>::test(tc, a);
        F<V, i+9>::test(tc, a);
        F<V, i+10>::test(tc, a);
        F<V, i+11>::test(tc, a);
        F<V, i+12>::test(tc, a);
        F<V, i+13>::test(tc, a);
        F<V, i+14>::test(tc, a);
        F<V, i+15>::test(tc, a);
        F<V, i+16>::test(tc, a);
        F<V, i+17>::test(tc, a);
        F<V, i+18>::test(tc, a);
        F<V, i+19>::test(tc, a);
        F<V, i+20>::test(tc, a);
        F<V, i+20>::test(tc, a);
        F<V, i+21>::test(tc, a);
        F<V, i+22>::test(tc, a);
        F<V, i+23>::test(tc, a);
        F<V, i+24>::test(tc, a);
        F<V, i+25>::test(tc, a);
        F<V, i+26>::test(tc, a);
        F<V, i+27>::test(tc, a);
        F<V, i+28>::test(tc, a);
        F<V, i+29>::test(tc, a);
        const bool is_large = i + 60 < limit;
        TemplateTestHelperImpl<F, V, is_large, i+30, limit>::run(tc, a);
    }

    static void run(TestSuite& tc,const V& a,const V& b)
    {
        F<V, i>::test(tc, a, b);
        F<V, i+1>::test(tc, a, b);
        F<V, i+2>::test(tc, a, b);
        F<V, i+3>::test(tc, a, b);
        F<V, i+4>::test(tc, a, b);
        F<V, i+5>::test(tc, a, b);
        F<V, i+6>::test(tc, a, b);
        F<V, i+7>::test(tc, a, b);
        F<V, i+8>::test(tc, a, b);
        F<V, i+9>::test(tc, a, b);
        F<V, i+10>::test(tc, a, b);
        F<V, i+11>::test(tc, a, b);
        F<V, i+12>::test(tc, a, b);
        F<V, i+13>::test(tc, a, b);
        F<V, i+14>::test(tc, a, b);
        F<V, i+15>::test(tc, a, b);
        F<V, i+16>::test(tc, a, b);
        F<V, i+17>::test(tc, a, b);
        F<V, i+18>::test(tc, a, b);
        F<V, i+19>::test(tc, a, b);
        F<V, i+20>::test(tc, a, b);
        F<V, i+20>::test(tc, a, b);
        F<V, i+21>::test(tc, a, b);
        F<V, i+22>::test(tc, a, b);
        F<V, i+23>::test(tc, a, b);
        F<V, i+24>::test(tc, a, b);
        F<V, i+25>::test(tc, a, b);
        F<V, i+26>::test(tc, a, b);
        F<V, i+27>::test(tc, a, b);
        F<V, i+28>::test(tc, a, b);
        F<V, i+29>::test(tc, a, b);
        const bool is_large = i + 60 < limit;
        TemplateTestHelperImpl<F, V, is_large, i+30, limit>::run(tc, a, b);
    }
};

template<template<class, unsigned> class F, class V, unsigned i>
struct TemplateTestHelperImpl<F, V, true, i, i> {
    static void run(TestSuite, const V&) {}
    static void run(TestSuite, const V&, const V&) {}
};

template<template<class, unsigned> class F, class V, unsigned i>
struct TemplateTestHelperImpl<F, V, false, i, i> {
    static void run(TestSuite, const V&) {}
    static void run(TestSuite, const V&, const V&) {}
};

template<template<class, unsigned> class F, class V>
struct TemplateTestHelper {
    static void run(TestSuite& tc,const V& a)
    {
        const unsigned limit = F<V,0>::limit;

        tc.reset_seq();
        TemplateTestHelperImpl<F, V, false, 0, limit>::run(tc, a);
    }

    static void run(TestSuite& tc,const V& a,const V& b)
    {
        const unsigned limit = F<V,0>::limit;

        tc.reset_seq();
        TemplateTestHelperImpl<F, V, false, 0, limit>::run(tc, a, b);
    }
};

template<template<class, unsigned> class F, class V>
struct TemplateTestArrayHelper {
    static void run(TestSuite& tc, V* a, unsigned n)
    {
        const unsigned limit = F<V,0>::limit;

        tc.reset_seq();
        for (unsigned i = 0; i < n; i++) {
            TemplateTestHelperImpl<F, V, false, 0, limit>::run(tc, *(a+i));
        }
    }

    static void run(TestSuite& tc, V* a, V* b, unsigned n)
    {
        const unsigned limit = F<V,0>::limit;

        tc.reset_seq();
        for (unsigned i = 0; i < n; i++) {
            TemplateTestHelperImpl<F, V, false, 0, limit>::run(tc, *(a+i), *(b+i));
        }
    }
};

template<class V1, class V2>
void test_cmp_vectors(SeqTestSuite& ts, const V1& q1, const V2& q2,
                      unsigned line, const char* file)
{
    typename simdpp::detail::get_expr_nomask<V1>::type v1 = q1.eval();
    typename simdpp::detail::get_expr_nomask<V2>::type v2 = q2.eval();
    static_assert(sizeof(v1) == sizeof(v2),
                  "Only vectors of same size should be compared");

    bool success = std::memcmp(&v1, &v2, sizeof(v1)) == 0;
    ts.add_result(success);

    if (!success) {
        std::cout << "FAIL at line " << line << " of " << file << std::endl;
    }
}

#define TEST_CMP_VEC(TS, V1, V2) do { test_cmp_vectors(TS, V1, V2, __LINE__, __FILE__); } while(0)

#endif
