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

#ifndef ALGEBRA_CORE_TYPES_RING_HH
#define ALGEBRA_CORE_TYPES_RING_HH

#include <concepts>
#include <type_traits>

namespace alg {

template <typename T>
concept ring = std::is_arithmetic_v<T> || requires (T t, T& tref, const T& ctref) {
    // operator+
    { t     + t     } -> std::same_as<T>;
    { t     + tref  } -> std::same_as<T>;
    { t     + ctref } -> std::same_as<T>;
    { tref  + t     } -> std::same_as<T>;
    { tref  + tref  } -> std::same_as<T>;
    { tref  + ctref } -> std::same_as<T>;
    { ctref + t     } -> std::same_as<T>;
    { ctref + tref  } -> std::same_as<T>;
    { ctref + ctref } -> std::same_as<T>;

    // operator+=
    { t    += t     } -> std::same_as<T&>;
    { t    += tref  } -> std::same_as<T&>;
    { t    += ctref } -> std::same_as<T&>;
    { tref += t     } -> std::same_as<T&>;
    { tref += tref  } -> std::same_as<T&>;
    { tref += ctref } -> std::same_as<T&>;

    // operator-
    { t     - t     } -> std::same_as<T>;
    { t     - tref  } -> std::same_as<T>;
    { t     - ctref } -> std::same_as<T>;
    { tref  - t     } -> std::same_as<T>;
    { tref  - tref  } -> std::same_as<T>;
    { tref  - ctref } -> std::same_as<T>;
    { ctref - t     } -> std::same_as<T>;
    { ctref - tref  } -> std::same_as<T>;
    { ctref - ctref } -> std::same_as<T>;

    // operator-=
    { t    -= t     } -> std::same_as<T&>;
    { t    -= tref  } -> std::same_as<T&>;
    { t    -= ctref } -> std::same_as<T&>;
    { tref -= t     } -> std::same_as<T&>;
    { tref -= tref  } -> std::same_as<T&>;
    { tref -= ctref } -> std::same_as<T&>;

    // operator*
    { t     * t     } -> std::same_as<T>;
    { t     * tref  } -> std::same_as<T>;
    { t     * ctref } -> std::same_as<T>;
    { tref  * t     } -> std::same_as<T>;
    { tref  * tref  } -> std::same_as<T>;
    { tref  * ctref } -> std::same_as<T>;
    { ctref * t     } -> std::same_as<T>;
    { ctref * tref  } -> std::same_as<T>;
    { ctref * ctref } -> std::same_as<T>;

    // operator*=
    { t    *= t     } -> std::same_as<T&>;
    { t    *= tref  } -> std::same_as<T&>;
    { t    *= ctref } -> std::same_as<T&>;
    { tref *= t     } -> std::same_as<T&>;
    { tref *= tref  } -> std::same_as<T&>;
    { tref *= ctref } -> std::same_as<T&>;

    /// The zero element in the ring.
    { T::zero() } noexcept -> std::same_as<T>;
    /// The one element in the ring.
    { T::one() } noexcept -> std::same_as<T>;
};

template <ring Ring>
Ring ring_zero() noexcept {
    if constexpr (!std::is_arithmetic_v<Ring>) {
        return Ring::zero();
    } else {
        return static_cast<Ring>(0);
    }
}

template <ring Ring>
Ring ring_one() noexcept {
    if constexpr (!std::is_arithmetic_v<Ring>) {
        return Ring::one();
    } else {
        return static_cast<Ring>(1);
    }
}

} // namespace alg

#endif // ALGEBRA_CORE_TYPES_RING_HH
