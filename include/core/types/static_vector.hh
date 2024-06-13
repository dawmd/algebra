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

#ifndef ALGEBRA_CORE_TYPES_STATIC_VECTOR_HH
#define ALGEBRA_CORE_TYPES_STATIC_VECTOR_HH

#include <core/types/field.hh>
#include <core/types/ring.hh>
#include <core/types/vector.hh>

#include <array>
#include <concepts>
#include <cstddef>
#include <span>
#include <utility>

namespace alg {

template <ring Ring, std::size_t Size>
    requires (Size > 0)
class static_vector {
private:
    alignas(Ring) std::array<Ring, Size> m_values;

public:
    static_vector() = default;
    static_vector(const static_vector&) = default;
    static_vector(static_vector&&) = default;

    template <typename T>
        requires std::convertible_to<T, Ring>
    static_vector(const T& init_value)
        : m_values{[&] {
            auto repeat = [] <std::size_t... Idxs> (const T& t, std::index_sequence<Idxs...>) {
                auto convert = [] <typename U, std::size_t> (U&& u) -> Ring {
                    return Ring(std::forward<U>(u));
                };
                return std::array<Ring, Size>{{convert.template operator()<decltype(t), Idxs>(t)...}};
            };
            return repeat(init_value, std::make_index_sequence<Size>());
        } ()}
    {}

    template <typename T>
        requires std::convertible_to<T, Ring>
    static_vector(std::span<T, Size> init_values)
        : m_values{[&] <std::size_t... Idxs> (std::index_sequence<Idxs...>) {
            auto convert = [] <typename U> (U&& u) -> Ring {
                return Ring(std::forward<U>(u));
            };
            return std::array<Ring, Size>{{convert(std::forward<T>(init_values[Idxs]))...}};
        } (std::make_index_sequence<Size>())}
    {}

    static_vector& operator=(const static_vector&) = default;
    static_vector& operator=(static_vector&&) = default;

    ~static_vector() = default;

public:
    auto&& operator[](this auto&& self, std::size_t idx) noexcept {
        return m_values[idx];
    }

    /// Vector-vector operations.
    static_vector operator+(const static_vector& other) const {
        return define_vec_op(*this, other, [] (const Ring& lhs, const Ring& rhs) { return lhs + rhs; });
    }
    static_vector& operator+=(const static_vector& other) {
        return define_vec_op_self(*this, other, [] (Ring& lhs, const Ring& rhs) { lhs += rhs; });
    }

    static_vector operator-(const static_vector& other) const {
        return define_vec_op(*this, other, [] (const Ring& lhs, const Ring& rhs) { return lhs - rhs; });
    }
    static_vector& operator-=(const static_vector& other) {
        return define_vec_op_self(*this, other, [] (Ring& lhs, const Ring& rhs) { lhs -= rhs; });
    }
    
    static_vector operator*(const static_vector& other) const {
        return define_vec_op(*this, other, [] (const Ring& lhs, const Ring& rhs) { return lhs * rhs; });
    }
    static_vector& operator*=(const static_vector& other) {
        return define_vec_op_self(*this, other, [] (Ring& lhs, const Ring& rhs) { lhs *= rhs; });
    }

    /// Scalar-vector operations.
    static_vector operator+(const Ring& r) const {
        return define_scalar_op(r, *this, [] (const Ring& lhs, const Ring& rhs) { return lhs + rhs; });
    }
    static_vector& operator+=(const Ring& r) {
        return define_scalar_op_self(r, *this, [] (const Ring& lhs, Ring& rhs) { rhs += lhs; });
    }
    friend static_vector operator+(const Ring& r, const static_vector& vec) {
        return vec + r;
    }

    static_vector operator-(const Ring& r) const {
        return define_scalar_op(r, *this, [] (const Ring& lhs, const Ring& rhs) { return lhs - rhs; });
    }
    static_vector& operator-=(const Ring& r) {
        return define_scalar_op_self(r, *this, [] (const Ring& lhs, Ring& rhs) { rhs -= lhs; });
    }
    friend static_vector operator+(const Ring& r, const static_vector& vec) {
        return vec - r;
    }

    static_vector operator*(const Ring& r) const {
        return define_scalar_op(r, *this, [] (const Ring& lhs, const Ring& rhs) { return lhs * rhs; });
    }
    static_vector& operator*=(const Ring& r) {
        return define_scalar_op_self(r, *this, [] (const Ring& lhs, Ring& rhs) { rhs *= lhs; });
    }
    friend static_vector operator*(const Ring& r, const static_vector& vec) {
        return vec * r;
    }

    template <typename = typename std::enable_if<field<Ring>>::type>
    static_vector operator/(const Ring& r) const {
        return define_scalar_op(r, *this, [] (const Ring& lhs, const Ring& rhs) { return rhs / lhs; });
    }
    template <typename = typename std::enable_if<field<Ring>>::type>
    static_vector& operator/=(const Ring& r) const {
        return define_scalar_op_self(r, *this, [] (const Ring& lhs, Ring& rhs) { rhs /= lhs; });
    }

    /// Miscellaneous.
    [[gnu::const]] Ring dot(const static_vector& other) const {
        Ring result = ring_zero<Ring>();
        for (std::size_t idx = 0; idx < Size; ++idx) {
            result += m_values[idx] * other.m_values[idx];
        }
        return result;
    }

public:
    static static_vector zero() noexcept {
        return static_vector{Ring::zero()};
    }
    static static_vector one() noexcept {
        return static_vector{Ring::one()};
    }

private:
    template <typename Op>
    static static_vector define_vec_op(const static_vector& lhs, const static_vector& rhs, const Op& op) {
        return [&] <std::size_t... Idxs> (std::index_sequence<Idxs...>) {
            return std::array<Ring, Size>{{op(lhs.m_values[Idxs], rhs.m_values[Idxs])...}};
        } (std::make_index_sequence<Size>());
    }

    template <typename Op>
    static static_vector& define_vec_op_self(static_vector& lhs, const static_vector& rhs, const Op& op) {
        for (std::size_t i = 0; i < Size; ++i) {
            op(lhs.m_values[i], rhs.m_values[i]);
        }
        return lhs;
    }

    template <typename Op>
    static static_vector define_scalar_op(const Ring& r, const static_vector& vec, const Op& op) {
        return [&] <std::size_t... Idxs> (std::index_sequence<Idxs...>) {
            return std::array<Ring, Size>{{op(r, vec.m_values[Idxs])...}};
        } (std::make_index_sequence<Size>());
    }

    template <typename Op>
    static static_vector& define_scalar_op_self(const Ring& r, static_vector& vec, const Op& op) {
        for (std::size_t i = 0; i < Size; ++i) {
            op(r, vec.m_values[i]);
        }
        return vec;
    }
};

namespace detail {

template <ring Ring, std::size_t Size>
struct is_vector<static_vector<Ring, Size>> {
    static constexpr bool value = true;
};

} // namespace detail

template <typename T, std::size_t Size>
    requires (Size != std::dynamic_extent)
static_vector(std::span<T, Size>) -> static_vector<T, Size>;

} // namespace alg

#endif // ALGEBRA_CORE_TYPES_STATIC_VECTOR_HH
