/*  Copyright (C) 2011-2014  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef LIBSIMDPP_SIMDPP_DETAIL_INSN_BROADCAST_W_H
#define LIBSIMDPP_SIMDPP_DETAIL_INSN_BROADCAST_W_H

#ifndef LIBSIMDPP_SIMD_H
    #error "This file must be included through simd.h"
#endif

#include <simdpp/types.h>
#include <simdpp/core/splat_n.h>
#include <simdpp/sse/extract_half.h>
#include <simdpp/detail/insn/shuffle128.h>

namespace simdpp {
#ifndef SIMDPP_DOXYGEN
namespace SIMDPP_ARCH_NAMESPACE {
#endif
namespace detail {
namespace insn {

// template<unsigned s, class V> SIMDPP_INL
// V v_splat(V a);

// -----------------------------------------------------------------------------

template<unsigned s> SIMDPP_INL
uint8x16 i_splat(const uint8x16& a)
{
    return i_splat16<s>(a);
}

#if SIMDPP_USE_AVX2
template<unsigned s> SIMDPP_INL
uint8x32 i_splat(const uint8x32& a)
{
    static_assert(s < 32, "Access out of bounds");
    uint8x16 lo;
    lo = s < 16 ? sse::extract_lo(a) : sse::extract_hi(a);
    lo = move16_l<s % 16>(lo);
    return _mm256_broadcastb_epi8(lo);
}
#endif

template<unsigned s, unsigned N> SIMDPP_INL
uint8<N> i_splat(const uint8<N>& a)
{
    static_assert(s < N, "Access out of bounds");
    return v_splat<s>(a);
}

// -----------------------------------------------------------------------------

template<unsigned s> SIMDPP_INL
uint16x8 i_splat(const uint16x8& a)
{
    return i_splat8<s>(a);
}

#if SIMDPP_USE_AVX2
template<unsigned s> SIMDPP_INL
uint16x16 i_splat(const uint16x16& a)
{
    static_assert(s < 16, "Access out of bounds");
    uint16x8 lo;
    lo = s < 8 ? sse::extract_lo(a) : sse::extract_hi(a);
    lo = move8_l<s % 8>(lo);
    return _mm256_broadcastw_epi16(lo);
}
#endif

template<unsigned s, unsigned N> SIMDPP_INL
uint16<N> i_splat(const uint16<N>& a)
{
    static_assert(s < N, "Access out of bounds");
    return v_splat<s>(a);
}

// -----------------------------------------------------------------------------

template<unsigned s> SIMDPP_INL
uint32x4 i_splat(const uint32x4& a)
{
    return i_splat4<s>(a);
}

#if SIMDPP_USE_AVX2
template<unsigned s> SIMDPP_INL
uint32x8 i_splat(const uint32x8& ca)
{
    static_assert(s < 8, "Access out of bounds");
    uint32<8> a = ca;
    a = permute4<s%4,s%4,s%4,s%4>(a);
    a = detail::shuffle1_128<s/4, s/4>(a, a);
    return a;
}
#endif

#if SIMDPP_USE_AVX512
template<unsigned s> SIMDPP_INL
uint32<16> i_splat(const uint32<16>& ca)
{
    static_assert(s < 16, "Access out of bounds");
    uint32<16> a = ca;
    a = permute4<s%4,s%4,s%4,s%4>(a);
    a = detail::shuffle2_128<s/4, s/4, s/4, s/4>(a, a);
    return a;
}
#endif

template<unsigned s, unsigned N> SIMDPP_INL
uint32<N> i_splat(const uint32<N>& a)
{
    static_assert(s < N, "Access out of bounds");
    return v_splat<s>(a);
}

// -----------------------------------------------------------------------------

template<unsigned s> SIMDPP_INL
uint64x2 i_splat(const uint64x2& a)
{
    return i_splat2<s>(a);
}

#if SIMDPP_USE_AVX2
template<unsigned s> SIMDPP_INL
uint64x4 i_splat(const uint64x4& a)
{
    static_assert(s < 4, "Access out of bounds");
    return permute4<s,s,s,s>(a);
}
#endif

#if SIMDPP_USE_AVX512
template<unsigned s> SIMDPP_INL
uint64<8> i_splat(const uint64<8>& ca)
{
    static_assert(s < 8, "Access out of bounds");
    uint64<8> a = ca;
    a = permute2<s%2,s%2>(a);
    a = _mm512_shuffle_i64x2(a, a, ((s/2) << 6) + ((s/2) << 4) + ((s/2) << 2) + (s/2)); // TODO extract
    return a;
}
#endif

template<unsigned s, unsigned N> SIMDPP_INL
uint64<N> i_splat(const uint64<N>& a)
{
    static_assert(s < N, "Access out of bounds");
    return v_splat<s>(a);
}

// -----------------------------------------------------------------------------

template<unsigned s> SIMDPP_INL
float32x4 i_splat(const float32x4& a)
{
    return i_splat4<s>(a);
}

#if SIMDPP_USE_AVX
template<unsigned s> SIMDPP_INL
float32x8 i_splat(const float32x8& ca)
{
    static_assert(s < 8, "Access out of bounds");
    float32<8> a = ca;
    a = shuffle1_128<s/4,s/4>(a, a);
    return permute4<s%4,s%4,s%4,s%4>(a);
}
#endif

#if SIMDPP_USE_AVX512
template<unsigned s> SIMDPP_INL
float32<16> i_splat(const float32<16>& ca)
{
    static_assert(s < 16, "Access out of bounds");
    float32<16> a = ca;
    a = permute4<s%4,s%4,s%4,s%4>(a);
    a = _mm512_shuffle_f32x4(a, a, s/4);
    return a;
}
#endif

template<unsigned s, unsigned N> SIMDPP_INL
float32<N> i_splat(const float32<N>& a)
{
    static_assert(s < N, "Access out of bounds");
    return v_splat<s>(a);
}

// -----------------------------------------------------------------------------

template<unsigned s> SIMDPP_INL
float64x2 i_splat(const float64x2& a)
{
    return i_splat2<s>(a);
}

#if SIMDPP_USE_AVX
template<unsigned s> SIMDPP_INL
float64x4 i_splat(const float64x4& a)
{
    static_assert(s < 4, "Access out of bounds");
#if SIMDPP_USE_AVX2
    return permute4<s,s,s,s>(a);
#else // SIMDPP_USE_AVX
    float64<4> b;
    b = detail::shuffle1_128<s/2,s/2>(a, a);
    b = permute2<s%2,s%2>(b);
    return b;
#endif
}
#endif

#if SIMDPP_USE_AVX512
template<unsigned s> SIMDPP_INL
float64<8> i_splat(const float64<8>& ca)
{
    static_assert(s < 8, "Access out of bounds");
    float64<8> a = ca;
    a = permute2<s%2,s%2>(a);
    a = _mm512_shuffle_f64x2(a, a, ((s/2) << 6) + ((s/2) << 4) + ((s/2) << 2) + (s/2)); // TODO extract
    return a;
}
#endif

template<unsigned s, unsigned N> SIMDPP_INL
float64<N> i_splat(const float64<N>& a)
{
    static_assert(s < N, "Access out of bounds");
    return v_splat<s>(a);
}

// -----------------------------------------------------------------------------

// collect some duplicate stuff here
template<unsigned s, class V> SIMDPP_INL
V v_splat(const V& a)
{
    using U = typename V::base_vector_type;
    U one = a.vec(s / U::length);

    one = i_splat<s % U::length>(one);

	auto tmp_a = a;
    for (unsigned i = 0; i < V::vec_length; ++i) {
		tmp_a.vec(i) = one;
    }
    return tmp_a;
}

} // namespace insn
} // namespace detail
#ifndef SIMDPP_DOXYGEN
} // namespace SIMDPP_ARCH_NAMESPACE
#endif
} // namespace simdpp

#endif

