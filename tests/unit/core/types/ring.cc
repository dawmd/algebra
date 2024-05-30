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

#include <core/types/ring.hh>
#include <core/types/types.hh>

#define CHECK_OK(type_name)     static_assert(    alg::ring<type_name>, #type_name " is a ring type")
#define CHECK_NOT_OK(type_name) static_assert(not alg::ring<type_name>, #type_name " is not a ring type")

struct add_only {
    add_only operator+(const add_only&);
};
struct subtract_only {
    add_only operator-(const add_only&);
};
struct multiply_only {
    add_only operator*(const add_only&);
};

struct all_except_plus {
    using T = all_except_plus;

    // T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct all_except_self_plus {
    using T = all_except_self_plus;

    T operator+(const T&) const;
    // T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct all_except_minus {
    using T = all_except_minus;

    T operator+(const T&) const;
    T& operator+=(const T&);
    // T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct all_except_self_minus {
    using T = all_except_self_minus;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    // T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct all_except_multiply {
    using T = all_except_multiply;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    // T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct all_except_self_multiply {
    using T = all_except_self_multiply;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    // T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct all_except_zero {
    using T = all_except_zero;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    // static T zero() noexcept;
    static T one();
};

struct all_except_one {
    using T = all_except_one;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    // static T one();
};

struct wrong_plus_type1 {
    using T = wrong_plus_type1;

    void operator+(const T&);
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_plus_type2 {
    using T = wrong_plus_type2;

    T& operator+(const T&);
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_self_plus_type1 {
    using T = wrong_self_plus_type1;

    T operator+(const T&) const;
    void operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_self_plus_type2 {
    using T = wrong_self_plus_type2;

    T operator+(const T&) const;
    T operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_minus_type1 {
    using T = wrong_minus_type1;

    T operator+(const T&) const;
    T& operator+=(const T&);
    void operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_minus_type2 {
    using T = wrong_minus_type2;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T& operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_self_minus_type1 {
    using T = wrong_self_minus_type1;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    void operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_self_minus_type2 {
    using T = wrong_self_minus_type2;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_multiply_type1 {
    using T = wrong_multiply_type1;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    void operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_multiply_type2 {
    using T = wrong_multiply_type2;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T& operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_self_multiply_type1 {
    using T = wrong_self_multiply_type1;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    void operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_self_multiply_type2 {
    using T = wrong_self_multiply_type2;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct wrong_zero_type1 {
    using T = wrong_zero_type1;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T operator*=(const T&);

    static void zero();
    static T one();
};

struct wrong_zero_type2 {
    using T = wrong_zero_type2;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T operator*=(const T&);

    static int zero();
    static T one();
};

struct wrong_zero_type3 {
    using T = wrong_zero_type3;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T operator*=(const T&);

    static T& zero();
    static T one();
};

struct wrong_zero_type4 {
    using T = wrong_zero_type4;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T operator*=(const T&);

    static const T& zero();
    static T one();
};

struct wrong_one_type1 {
    using T = wrong_one_type1;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T operator*=(const T&);

    static T zero() noexcept;
    static void one();
};

struct wrong_one_type2 {
    using T = wrong_one_type2;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T operator*=(const T&);

    static T zero() noexcept;
    static int one();
};

struct wrong_one_type3 {
    using T = wrong_one_type3;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T operator*=(const T&);

    static T zero() noexcept;
    static T& one();
};

struct wrong_one_type4 {
    using T = wrong_one_type4;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&);
    T operator*=(const T&);

    static T zero() noexcept;
    static const T& one();
};

struct non_const_plus {
    using T = non_const_plus;

    T operator+(const T&);
    T& operator+=(const T&);
    T operator-(const T&) const;
    T& operator-=(const T&);
    T operator*(const T&) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct non_const_minus {
    using T = non_const_plus;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&);
    T& operator-=(const T&);
    T operator*(const T&) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct non_const_multiply {
    using T = non_const_plus;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&) const;
    T& operator-=(const T&);
    T operator*(const T&);
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct non_noexcept_zero {
    using T = non_noexcept_zero;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&) const;
    T& operator-=(const T&);
    T operator*(const T&) const;
    T& operator*=(const T&);

    static T zero();
    static T one() noexcept;
};

struct non_noexcept_one {
    using T = non_noexcept_one;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&) const;
    T& operator-=(const T&);
    T operator*(const T&) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one();
};

struct not_ok_plus {
    using T = not_ok_plus;

    T operator+(T&) const;
    T& operator+=(const T&);
    T operator-(const T&) const;
    T& operator-=(const T&);
    T operator*(const T&) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one() noexcept;
};

struct not_ok_minus {
    using T = not_ok_minus;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(T&) const;
    T& operator-=(const T&);
    T operator*(const T&) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one() noexcept;
};

struct not_ok_multiply {
    using T = not_ok_multiply;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&) const;
    T& operator-=(const T&);
    T operator*(T&) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one() noexcept;
};


struct empty_struct {};

struct all_ok {
    using T = all_ok;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&) const;
    T& operator-=(const T&);
    T operator*(const T&) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one() noexcept;
};

struct ok_plus {
    using T = ok_plus;

    T operator+(T) const;
    T& operator+=(const T&);
    T operator-(const T&) const;
    T& operator-=(const T&);
    T operator*(const T&) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one() noexcept;
};

struct ok_self_plus {
    using T = ok_self_plus;

    T operator+(const T&) const;
    T& operator+=(T);
    T operator-(const T&) const;
    T& operator-=(const T&);
    T operator*(const T&) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one() noexcept;
};

struct ok_minus {
    using T = ok_minus;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(T) const;
    T& operator-=(const T&);
    T operator*(const T&) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one() noexcept;
};

struct ok_self_minus {
    using T = ok_self_minus;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&) const;
    T& operator-=(T);
    T operator*(const T&) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one() noexcept;
};

struct ok_multiply {
    using T = ok_multiply;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&) const;
    T& operator-=(const T&);
    T operator*(T) const;
    T& operator*=(const T&);

    static T zero() noexcept;
    static T one() noexcept;
};

struct ok_self_multiply {
    using T = ok_self_multiply;

    T operator+(const T&) const;
    T& operator+=(const T&);
    T operator-(const T&) const;
    T& operator-=(const T&);
    T operator*(const T&) const;
    T& operator*=(T);

    static T zero() noexcept;
    static T one() noexcept;
};

int main() {
    CHECK_NOT_OK(alg::byte);

    CHECK_OK(alg::i8);
    CHECK_OK(alg::i16);
    CHECK_OK(alg::i32);
    CHECK_OK(alg::i64);
    CHECK_OK(alg::u8);
    CHECK_OK(alg::u16);
    CHECK_OK(alg::u32);
    CHECK_OK(alg::u64);

    CHECK_OK(alg::fast_i8);
    CHECK_OK(alg::fast_i16);
    CHECK_OK(alg::fast_i32);
    CHECK_OK(alg::fast_i64);
    CHECK_OK(alg::fast_u8);
    CHECK_OK(alg::fast_u16);
    CHECK_OK(alg::fast_u32);
    CHECK_OK(alg::fast_u64);

    CHECK_OK(alg::least_i8);
    CHECK_OK(alg::least_i16);
    CHECK_OK(alg::least_i32);
    CHECK_OK(alg::least_i64);
    CHECK_OK(alg::least_u8);
    CHECK_OK(alg::least_u16);
    CHECK_OK(alg::least_u32);
    CHECK_OK(alg::least_u64);

    CHECK_OK(alg::max_int_type);
    CHECK_OK(alg::max_uint_type);

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

    CHECK_NOT_OK(add_only);
    CHECK_NOT_OK(subtract_only);
    CHECK_NOT_OK(multiply_only);

    CHECK_NOT_OK(all_except_plus);
    CHECK_NOT_OK(all_except_self_plus);
    CHECK_NOT_OK(all_except_minus);
    CHECK_NOT_OK(all_except_self_minus);
    CHECK_NOT_OK(all_except_multiply);
    CHECK_NOT_OK(all_except_self_minus);
    CHECK_NOT_OK(all_except_zero);
    CHECK_NOT_OK(all_except_one);

    CHECK_NOT_OK(wrong_plus_type1);
    CHECK_NOT_OK(wrong_plus_type2);
    CHECK_NOT_OK(wrong_self_plus_type1);
    CHECK_NOT_OK(wrong_self_plus_type2);
    CHECK_NOT_OK(wrong_minus_type1);
    CHECK_NOT_OK(wrong_minus_type2);
    CHECK_NOT_OK(wrong_self_minus_type1);
    CHECK_NOT_OK(wrong_self_minus_type2);
    CHECK_NOT_OK(wrong_multiply_type1);
    CHECK_NOT_OK(wrong_multiply_type2);
    CHECK_NOT_OK(wrong_self_multiply_type1);
    CHECK_NOT_OK(wrong_self_multiply_type2);
    CHECK_NOT_OK(wrong_zero_type1);
    CHECK_NOT_OK(wrong_zero_type2);
    CHECK_NOT_OK(wrong_zero_type3);
    CHECK_NOT_OK(wrong_zero_type4);
    CHECK_NOT_OK(wrong_one_type1);
    CHECK_NOT_OK(wrong_one_type2);
    CHECK_NOT_OK(wrong_one_type3);
    CHECK_NOT_OK(wrong_one_type4);

    CHECK_NOT_OK(non_const_plus);
    CHECK_NOT_OK(non_const_minus);
    CHECK_NOT_OK(non_const_multiply);

    CHECK_NOT_OK(non_noexcept_zero);
    CHECK_NOT_OK(non_noexcept_one);

    CHECK_NOT_OK(not_ok_plus);
    CHECK_NOT_OK(not_ok_minus);
    CHECK_NOT_OK(not_ok_multiply);
    
    CHECK_NOT_OK(empty_struct);

    CHECK_OK(all_ok);
    CHECK_OK(ok_plus);
    CHECK_OK(ok_self_plus);
    CHECK_OK(ok_minus);
    CHECK_OK(ok_self_minus);
    CHECK_OK(ok_multiply);
    CHECK_OK(ok_self_multiply);
}
