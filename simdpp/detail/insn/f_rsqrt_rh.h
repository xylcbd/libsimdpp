/*  Copyright (C) 2013-2014  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef LIBSIMDPP_SIMDPP_DETAIL_INSN_F_RSQRT_RH_H
#define LIBSIMDPP_SIMDPP_DETAIL_INSN_F_RSQRT_RH_H

#ifndef LIBSIMDPP_SIMD_H
    #error "This file must be included through simd.h"
#endif

#include <cmath>
#include <simdpp/types.h>
#include <simdpp/core/f_mul.h>
#include <simdpp/core/f_sub.h>
#include <simdpp/detail/null/foreach.h>
#include <simdpp/detail/null/math.h>

namespace simdpp {
#ifndef SIMDPP_DOXYGEN
namespace SIMDPP_ARCH_NAMESPACE {
#endif
namespace detail {
namespace insn {

template<class V> SIMDPP_INL
V v_rsqrt_rh(const V& x,const V& a)
{
    V x2, r;

    x2 = mul(x, x);
    r = mul(a, x2);
    r = sub(3.0, r);
    const auto tmp_x = mul(x, 0.5);
    r = mul(tmp_x, r);

    return r;
}

SIMDPP_INL float32x4 i_rsqrt_rh(const float32x4& cx, const float32x4& a)
{
    // x_n = x*(3-d*x*x)/2
    float32<4> x = cx;
#if SIMDPP_USE_NULL || SIMDPP_USE_NEON_NO_FLT_SP
    return detail::null::foreach<float32x4>(x, a, [](float x, float a){ return x * (3.0f - a*x*x) * 0.5f; });
#elif SIMDPP_USE_SSE2
    return v_rsqrt_rh(x, a);
#elif SIMDPP_USE_NEON_FLT_SP
    float32x4 x2, r;

    x2 = mul(x, x);
    r = vrsqrtsq_f32(a, x2);
    x = mul(x, r);

    return x;
#elif SIMDPP_USE_ALTIVEC
    float32x4 x2, r, xp5, c3;

    c3 = make_float(3.0f);

    x2 = mul(x, x);
    // r = (c3 - a*x2)
    r = vec_nmsub((__vector float)a, (__vector float)x2, (__vector float)c3);
    xp5 = mul(x, 0.5);
    r = mul(xp5, r);

    return r;
#endif
}

#if SIMDPP_USE_AVX
SIMDPP_INL float32x8 i_rsqrt_rh(const float32x8& x, const float32x8& a)
{
    return v_rsqrt_rh(x, a);
}
#endif

#if SIMDPP_USE_AVX512
SIMDPP_INL float32<16> i_rsqrt_rh(const float32<16>& x, const float32<16>& a)
{
    return v_rsqrt_rh(x, a);
}
#endif


template<unsigned N> SIMDPP_INL
float32<N> i_rsqrt_rh(const float32<N>& x, const float32<N>& a)
{
    SIMDPP_VEC_ARRAY_IMPL2(float32<N>, i_rsqrt_rh, x, a);
}

} // namespace insn
} // namespace detail
#ifndef SIMDPP_DOXYGEN
} // namespace SIMDPP_ARCH_NAMESPACE
#endif
} // namespace simdpp

#endif

