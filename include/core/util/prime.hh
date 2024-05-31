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

#ifndef CORE_UTIL_PRIME_HH
#define CORE_UTIL_PRIME_HH

#include <core/types/types.hh>

#include <cmath>

namespace alg {

constexpr bool is_prime(u64 number) noexcept {
    // TODO: Primitive implementation. Improve.
    if (number < 2) {
        return false;
    }

    const auto limit = [&] {
        if consteval {
            return number - 1;
        } else {
            return static_cast<u64>(std::sqrt(number));
        }
    } ();

    for (u64 i = 2; i <= limit; ++i) {
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}

} // namespace alg

#endif // CORE_UTIL_PRIME_HH
