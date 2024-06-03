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

#include <cassert>
#include <concepts>
#include <cstring>
#include <print>
#include <string_view>
#include <type_traits>

namespace {

template <typename T>
    requires std::is_arithmetic_v<T>
bool bit_cmp_arithmetic(T lhs, T rhs) {
    return std::memcmp(std::addressof(lhs), std::addressof(rhs), sizeof(lhs));
}

} // anonymous namespace

#define CHECK(condition, ...)                                                                       \
    do {                                                                                            \
        if (!(condition)) {                                                                         \
            std::print("{} failed" __VA_OPT__(": {}") "\n", __func__ __VA_OPT__(, __VA_ARGS__));    \
        }                                                                                           \
    } while (false)

#define TEST_OK() std::print("{} ok\n", __func__)

namespace {

template <alg::ring Ring>
    requires std::is_arithmetic_v<Ring>
void basic_arithmetic() {
    auto x = alg::ring_zero<Ring>();
    static_assert(std::same_as<decltype(x), Ring>, "x is of wrong type");
    CHECK(bit_cmp_arithmetic(x, static_cast<Ring>(0)), "x should be equal to 0.0");

    auto y = alg::ring_one<Ring>();
    static_assert(std::same_as<decltype(y), Ring>, "y should be of type float");
    CHECK(bit_cmp_arithmetic(x, static_cast<Ring>(1)), "y should be equal to 1.0");
}

void test_simple_struct() {
    struct my_struct {
        int x;

        my_struct operator+(const my_struct&) const;
        my_struct& operator+=(const my_struct&);
        my_struct operator-(const my_struct&) const;
        my_struct& operator-=(const my_struct&);
        my_struct operator*(const my_struct&) const;
        my_struct& operator*=(const my_struct&);

        static my_struct zero() noexcept {
            return my_struct {.x = 0};
        }

        static my_struct one() noexcept {
            return my_struct {.x = 1};
        }
    };

    auto zero = alg::ring_zero<my_struct>();
    static_assert(std::same_as<decltype(zero), my_struct>, "zero is of wrong type");
    CHECK(zero.x == 0, "zero.x should be equal to 0");

    auto one = alg::ring_one<my_struct>();
    static_assert(std::same_as<decltype(one), my_struct>, "one is of wrong type");
    CHECK(one.x == 1, "one.x should be equal to 1");
}

} // anonymous namespace

int main() {
    basic_arithmetic<alg::u8>();
    basic_arithmetic<alg::u16>();
    basic_arithmetic<alg::u32>();
    basic_arithmetic<alg::u64>();

    basic_arithmetic<alg::i8>();
    basic_arithmetic<alg::i16>();
    basic_arithmetic<alg::i32>();
    basic_arithmetic<alg::i64>();

    basic_arithmetic<float>();
    basic_arithmetic<double>();

    // TODO: long double has padding bits of arbitrary values.
    // basic_arithmetic<long double>();

#if defined(__STDCPP_FLOAT16_T__)
    basic_arithmetic<alg::f16>();
#endif

#if defined(__STDCPP_FLOAT32_T__)
    basic_arithmetic<alg::f32>();
#endif

#if defined(__STDCPP_FLOAT64_T__)
    basic_arithmetic<alg::f64>();
#endif

#if defined(__STDCPP_FLOAT128_T__)
    basic_arithmetic<alg::f128>();
#endif

#if defined(__STDCPP_BFLOAT16_T__)
    basic_arithmetic<alg::bf16>();
#endif
}
