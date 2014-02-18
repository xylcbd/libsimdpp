/*  libsimdpp
    Copyright (C) 2013  Povilas Kanapickas povilas@radix.lt
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef LIBSIMDPP_SIMDPP_FUNCTIONS_F_CEIL_H
#define LIBSIMDPP_SIMDPP_FUNCTIONS_F_CEIL_H

#ifndef LIBSIMDPP_SIMD_H
    #error "This file must be included through simd.h"
#endif

#include <simdpp/simd/types.h>
#include <simdpp/functions/bit_or.h>
#include <simdpp/functions/blend.h>
#include <simdpp/functions/cmp_eq.h>
#include <simdpp/functions/cmp_gt.h>
#include <simdpp/functions/i_add.h>
#include <simdpp/functions/i_shift_r.h>
#include <simdpp/functions/i_sub.h>
#include <simdpp/functions/f_abs.h>
#include <simdpp/functions/to_float32.h>
#include <simdpp/functions/to_int32.h>
#include <simdpp/null/foreach.h>

namespace simdpp {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace SIMDPP_ARCH_NAMESPACE {
#endif


/// @{
/** Rounds the values a vector towards positive infinity

    @code
    r0 = ceil(a0)
    ...
    rN = ceil(aN)
    @endcode

    @par 128-bit version:
    @icost{SSE2, SSE3, SSSE3, 13-15}
    @icost{NEON, 11-13}

    @par 256-bit version:
    @icost{SSE2, SSE3, SSSE3, 26-28}
    @icost{NEON, 22-24}
    @icost{ALTIVEC, 2}
*/
inline float32x4 ceil(float32x4 a)
{
#if SIMDPP_USE_NULL || (SIMDPP_USE_NEON && !SIMDPP_USE_NEON_FLT_SP)
    return null::foreach<float32x4>(a, [](float x){ return std::ceil(x); });
#elif SIMDPP_USE_SSE4_1
    return _mm_ceil_ps(a);
#elif SIMDPP_USE_SSE2 || SIMDPP_USE_NEON_FLT_SP
    //check if the value is not too large, or is zero
    float32x4 ba = abs(a);
    mask_float32x4 large_mask, zero_mask, mask;
    large_mask = cmp_gt(ba, float32x4::make_const(8388607.0f));
    zero_mask = cmp_eq(ba, float32x4::zero());
    mask = bit_or(large_mask, zero_mask); // takes care of nans and zeros

    //calculate the ceil using trunc
    int32x4 s = shift_r((uint32x4)a, 31);
            s = bit_xor(s, int32x4::make_const(0x00000001)); //=1 if a>0
    float32x4 at = (float32x4) sub((int32x4)a, s); //=nextafter towards -inf if a>0
    int32x4 ia = to_int32x4(at);
            ia = add(ia, s);
    float32x4 fa = to_float32x4(ia);

    //combine the results
    a = blend(a, fa, mask);
    return a;
#elif SIMDPP_USE_ALTIVEC
    return vec_ceil((__vector float)a);
#endif
}

inline float32x8 ceil(float32x8 a)
{
#if SIMDPP_USE_AVX
    return _mm256_ceil_ps(a);
#else
    return {ceil(a[0]), ceil(a[1])};
#endif
}
/// @}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
} // namespace SIMDPP_ARCH_NAMESPACE
#endif
} // namespace simdpp

#endif

