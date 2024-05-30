// MIT License
//
// Copyright (c) 2024 Dawid Mędrek
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef ALGEBRA_CORE_TYPES_TYPES_HH
#define ALGEBRA_CORE_TYPES_TYPES_HH

#include <cstddef>
#include <cstdint>
#include <stdfloat>

namespace alg {

using byte = std::byte;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using fast_i8 = std::int_fast8_t;
using fast_i16 = std::int_fast16_t;
using fast_i32 = std::int_fast32_t;
using fast_i64 = std::int_fast64_t;

using fast_u8 = std::uint_fast8_t;
using fast_u16 = std::uint_fast16_t;
using fast_u32 = std::uint_fast32_t;
using fast_u64 = std::uint_fast64_t;

using least_i8 = std::int_least8_t;
using least_i16 = std::int_least16_t;
using least_i32 = std::int_least32_t;
using least_i64 = std::int_least64_t;

using least_u8 = std::uint_least8_t;
using least_u16 = std::uint_least16_t;
using least_u32 = std::uint_least32_t;
using least_u64 = std::uint_least64_t;

using max_int_type = std::intmax_t;
using max_uint_type = std::uintmax_t;

#if defined(__STDCPP_FLOAT16_T__)
using f16 = std::float16_t;
#endif

#if defined(__STDCPP_FLOAT32_T__)
using f32 = std::float32_t;
#endif

#if defined(__STDCPP_FLOAT64_T__)
using f64 = std::float64_t;
#endif

#if defined(__STDCPP_FLOAT128_T__)
using f128 = std::float128_t;
#endif

#if defined(__STDCPP_BFLOAT16_T__)
using bf16 = std::bfloat16_t;
#endif

} // namespace alg

#endif // ALGEBRA_CORE_TYPES_TYPES_HH
