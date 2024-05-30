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

#include <core/types/field.hh>
#include <core/types/types.hh>

#define CHECK_OK(type_name)     static_assert(    alg::field<type_name>, #type_name " is a field type")
#define CHECK_NOT_OK(type_name) static_assert(not alg::field<type_name>, #type_name " is not a field type")

int main() {
    CHECK_NOT_OK(alg::byte);

    CHECK_NOT_OK(alg::i8);
    CHECK_NOT_OK(alg::i16);
    CHECK_NOT_OK(alg::i32);
    CHECK_NOT_OK(alg::i64);
    CHECK_NOT_OK(alg::u8);
    CHECK_NOT_OK(alg::u16);
    CHECK_NOT_OK(alg::u32);
    CHECK_NOT_OK(alg::u64);

    CHECK_NOT_OK(alg::fast_i8);
    CHECK_NOT_OK(alg::fast_i16);
    CHECK_NOT_OK(alg::fast_i32);
    CHECK_NOT_OK(alg::fast_i64);
    CHECK_NOT_OK(alg::fast_u8);
    CHECK_NOT_OK(alg::fast_u16);
    CHECK_NOT_OK(alg::fast_u32);
    CHECK_NOT_OK(alg::fast_u64);

    CHECK_NOT_OK(alg::least_i8);
    CHECK_NOT_OK(alg::least_i16);
    CHECK_NOT_OK(alg::least_i32);
    CHECK_NOT_OK(alg::least_i64);
    CHECK_NOT_OK(alg::least_u8);
    CHECK_NOT_OK(alg::least_u16);
    CHECK_NOT_OK(alg::least_u32);
    CHECK_NOT_OK(alg::least_u64);

    CHECK_NOT_OK(alg::max_int_type);
    CHECK_NOT_OK(alg::max_uint_type);

    CHECK_OK(float);
    CHECK_OK(double);
    CHECK_OK(long double);

#if defined(__STDCPP_FLOAT16_T__)
    CHECK_OK(alg::f16);
#endif

#if defined(__STDCPP_FLOAT32_T__)
    CHECK_OK(alg::f32);
#endif

#if defined(__STDCPP_FLOAT64_T__)
    CHECK_OK(alg::f64);
#endif

#if defined(__STDCPP_FLOAT128_T__)
    CHECK_OK(alg::f128);
#endif

#if defined(__STDCPP_BFLOAT16_T__)
    CHECK_OK(alg::bf16);
#endif
}
