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

#ifndef ALGEBRA_CORE_TYPES_DYNAMIC_VECTOR_HH
#define ALGEBRA_CORE_TYPES_DYNAMIC_VECTOR_HH

#include <core/types/field.hh>
#include <core/types/ring.hh>
#include <core/types/vector.hh>
#include <detail/macros.hh>

#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <new>
#include <ranges>
#include <utility>

namespace alg {

template <ring Ring>
class dynamic_vector {
public:
    using value_type = Ring;
    using size_type = std::size_t;

private:
    Ring* m_values;
    Ring* m_end;

private:
    struct private_constructor_marker {};

    dynamic_vector(Ring* values, Ring* end, private_constructor_marker)
        : m_values{values}
        , m_end{end}
    {}

public:    
    dynamic_vector() = delete;

    dynamic_vector(const dynamic_vector& other) {
        m_values = (Ring*) std::assume_aligned<alignof(Ring)>(allocate_uninitialized(other.size()));
        m_end = m_values + other.size();

        std::uninitialized_copy(other.m_values, other.m_end, m_values);
    }

    dynamic_vector& operator=(const dynamic_vector& other) {
        if (this != std::addressof(other)) {
            std::destroy_at(this);
            std::construct_at(this, other);
        }
        return *this;
    }

    dynamic_vector(dynamic_vector&& other) {
        m_values = std::exchange(other.m_values, nullptr);
        m_end = other.m_end;
    }

    dynamic_vector& operator=(dynamic_vector&& other) {
        if (this != std::addressof(other)) {
            std::destroy_at(this);
            std::construct_at(this, std::move(other));
        }
        return *this;
    }

    dynamic_vector(size_type size, const Ring& init_value) {
        assert(size > 0);
        m_values = (Ring*) std::assume_aligned<alignof(Ring)>(allocate_uninitialized(size));
        m_end = m_values + size;

        std::uninitialized_fill(m_values, m_end, init_value);
    }

    template <std::input_iterator It>
        requires std::convertible_to<std::iter_value_t<It>, Ring>
    dynamic_vector(It begin, It end) {
        const auto size = std::distance(begin, end);
        assert(begin != end);

        m_values = (Ring*) std::assume_aligned<alignof(Ring)>(allocate_uninitialized(size));
        m_end = m_values;
        
        try {
            for (It it = begin; it != end; ++it) {
                std::construct_at(m_end, *it);
                ++m_end;
            }
        } catch (...) {
            std::ranges::destroy(std::views::reverse(std::ranges::subrange(m_values, m_end)));
            std::free(m_values);
            throw;
        }
    }

    template <std::ranges::range Range>
        requires std::convertible_to<std::ranges::range_value_t<Range>, Ring>
    dynamic_vector(std::from_range_t, Range&& range) {
        const auto size = std::distance(std::forward<Range>(range));

        m_values = (Ring*) std::assume_aligned<alignof(Ring)>(allocate_uninitialized(size));
        m_end = m_values;

        try {
            for (auto&& val : std::forward<Range>(range)) {
                std::construct_at(m_end, std::forward<decltype(val)>(val));
                ++m_end;
            }
        } catch (...) {
            std::ranges::destroy(std::views::reverse(std::ranges::subrange(m_values, m_end)));
            std::free(m_values);
            throw;
        }
    }

    ~dynamic_vector() noexcept {
        if (m_values) {
            std::ranges::destroy(std::views::reverse(std::ranges::subrange(m_values, m_end)));
            std::free(m_values);
        }
    }

public:
    auto* data(this auto&& self) noexcept {
        return std::assume_aligned<alignof(Ring)>(self.m_values);
    }

    auto&& operator[](this auto&& self, size_type idx) noexcept {
        return self.data()[idx];
    }

    [[gnu::const]] size_type size() const noexcept {
        return static_cast<size_type>(m_end - m_values);
    }

    /// Vector-vector operations.
    dynamic_vector operator+(const dynamic_vector& other) const {
        return define_vec_op(*this, other, [] (const Ring& lhs, const Ring& rhs) { return lhs + rhs; });
    }
    dynamic_vector& operator+=(const dynamic_vector& other) {
        return define_vec_op_self(*this, other, [] (Ring& lhs, const Ring& rhs) { lhs += rhs; });
    }

    dynamic_vector operator-(const dynamic_vector& other) const {
        return define_vec_op(*this, other, [] (const Ring& lhs, const Ring& rhs) { return lhs - rhs; });
    }
    dynamic_vector& operator-=(const dynamic_vector& other) {
        return define_vec_op_self(*this, other, [] (Ring& lhs, const Ring& rhs) { lhs -= rhs; });
    }
    
    dynamic_vector operator*(const dynamic_vector& other) const {
        return define_vec_op(*this, other, [] (const Ring& lhs, const Ring& rhs) { return lhs * rhs; });
    }
    dynamic_vector& operator*=(const dynamic_vector& other) {
        return define_vec_op_self(*this, other, [] (Ring& lhs, const Ring& rhs) { lhs *= rhs; });
    }

    /// Scalar-vector operations.
    dynamic_vector operator+(const Ring& r) const {
        return define_vec_scalar_op(*this, r, [] (const Ring& lhs, const Ring& rhs) { return lhs + rhs; });
    }
    dynamic_vector& operator+=(const Ring& r) {
        return define_vec_scalar_op_self(*this, r, [] (Ring& lhs, const Ring& rhs) { lhs += rhs; });
    }
    friend dynamic_vector operator+(const Ring& r, const dynamic_vector& vec) {
        return define_scalar_vec_op(r, vec, [] (const Ring& lhs, const Ring& rhs) { return lhs + rhs; });
    }

    dynamic_vector operator-(const Ring& r) const {
        return define_vec_scalar_op(*this, r, [] (const Ring& lhs, const Ring& rhs) { return lhs - rhs; });
    }
    dynamic_vector& operator-=(const Ring& r) {
        return define_vec_scalar_op_self(*this, r, [] (Ring& lhs, const Ring& rhs) { lhs -= rhs; });
    }
    friend dynamic_vector operator-(const Ring& r, const dynamic_vector& vec) {
        return define_scalar_vec_op(r, vec, [] (const Ring& lhs, const Ring& rhs) { return lhs - rhs; });
    }

    dynamic_vector operator*(const Ring& r) const {
        return define_vec_scalar_op(*this, r, [] (const Ring& lhs, const Ring& rhs) { return lhs * rhs; });
    }
    dynamic_vector& operator*=(const Ring& r) {
        return define_vec_scalar_op_self(*this, r, [] (Ring& lhs, const Ring& rhs) { lhs *= rhs; });
    }
    friend dynamic_vector operator*(const Ring& r, const dynamic_vector& vec) {
        return define_scalar_vec_op(r, vec, [] (const Ring& lhs, const Ring& rhs) { return lhs * rhs; });
    }

    dynamic_vector operator/(const Ring& r) const requires field<Ring> {
        return define_vec_scalar_op(*this, r, [] (const Ring& lhs, const Ring& rhs) { return lhs / rhs; });
    }
    dynamic_vector& operator/=(const Ring& r) requires field<Ring> {
        return define_vec_scalar_op_self(*this, r, [] (Ring& lhs, const Ring& rhs) { lhs /= rhs; });
    }
    friend dynamic_vector& operator/(const Ring& r, const dynamic_vector& vec) requires field<Ring> {
        return define_scalar_vec_op(r, vec, [] (const Ring& lhs, const Ring& rhs) { return lhs / rhs; });
    }

public:
    static dynamic_vector zero(size_type size) {
        assert(size > 0);
        return dynamic_vector(size, ring_zero<Ring>());
    }
    static dynamic_vector one(size_type size) {
        assert(size > 0);
        return dynamic_vector(size, ring_zero<Ring>());
    }

private:
    static Ring* allocate_uninitialized(size_type size) {
        Ring* result = (Ring*) std::aligned_alloc(alignof(Ring), sizeof(Ring) * size);
        if (result == nullptr) {
            throw std::bad_alloc{};
        }
        return std::assume_aligned<alignof(Ring)>(result);
    }

    template <typename Op>
        requires std::invocable<Op, const Ring&, const Ring&>
    static dynamic_vector define_vec_op(const dynamic_vector& lhs, const dynamic_vector& rhs, const Op& op) {
        assert(lhs.size() == rhs.size());
        
        const auto size = lhs.size();
        Ring* const M_ALG_RESTRICT data = std::assume_aligned<alignof(Ring)>(allocate_uninitialized(size));
        Ring* const M_ALG_RESTRICT ldata = lhs.data();
        Ring* const M_ALG_RESTRICT rdata = rhs.data();
        size_type idx = 0;

        try {
            for (; idx < size; ++idx) {
                std::construct_at(data + idx, op(ldata[idx], rdata[idx]));
            }
        } catch (...) {
            std::ranges::destroy(std::views::reverse(std::ranges::subrange(data, data + idx)));
            std::free(data);
            throw;
        }

        return dynamic_vector(data, data + size, private_constructor_marker{});
    }

    template <typename Op>
        requires std::invocable<Op, Ring&, const Ring&>
    static dynamic_vector& define_vec_op_self(dynamic_vector& lhs, const dynamic_vector& rhs, const Op& op) {
        const auto size = lhs.size();
        assert(size == rhs.size());

        Ring* M_ALG_RESTRICT ldata = lhs.data();
        Ring* M_ALG_RESTRICT rdata = rhs.data();

        for (size_type i = 0; i < size; ++i) {
            op(ldata[i], rdata[i]);
        }

        return lhs;
    }

    template <typename Op>
        requires std::invocable<Op, const Ring&, const Ring&>
    static dynamic_vector define_scalar_vec_op(const Ring& M_ALG_RESTRICT r, const dynamic_vector& vec, const Op& op) {
        const auto size = vec.size();
        Ring* const M_ALG_RESTRICT data = std::assume_aligned<alignof(Ring)>(allocate_uninitialized(size));
        Ring* const M_ALG_RESTRICT vecdata = vec.data();
        size_type idx = 0;

        try {
            for (; idx < size; ++idx) {
                std::construct_at(data + idx, op(r, vecdata[idx]));
            }
        } catch (...) {
            std::ranges::destroy(std::views::reverse(std::ranges::subrange(data, data + idx)));
            std::free(data);
            throw;
        }

        return dynamic_vector(data, data + size, private_constructor_marker{});
    }

    template <typename Op>
        requires std::invocable<Op, const Ring&, const Ring&>
    static dynamic_vector define_vec_scalar_op(const dynamic_vector& vec, const Ring& M_ALG_RESTRICT r, const Op& op) {
        const auto size = vec.size();
        Ring* const M_ALG_RESTRICT vecdata = vec.data();
        Ring* const M_ALG_RESTRICT data = std::assume_aligned<alignof(Ring)>(allocate_uninitialized(size));
        size_type idx = 0;

        try {
            for (; idx < size; ++idx) {
                std::construct_at(data + idx, op(vecdata[idx], r));
            }
        } catch (...) {
            std::ranges::destroy(std::views::reverse(std::ranges::subrange(data, data + idx)));
            std::free(data);
            throw;
        }

        return dynamic_vector(data, data + size, private_constructor_marker{});
    }

    template <typename Op>
        requires std::invocable<Op, Ring&, const Ring&>
    static dynamic_vector& define_vec_scalar_op_self(dynamic_vector& vec, const Ring& M_ALG_RESTRICT r, const Op& op) {
        const auto size = vec.size();
        Ring* const M_ALG_RESTRICT data = vec.data();

        for (size_type i = 0; i < size; ++i) {
            op(data[i], r);
        }

        return vec;
    }
};

namespace detail {

template <ring Ring>
struct is_vector<dynamic_vector<Ring>> {
    static constexpr bool value = true;
};

} // namespace detail

} // namespace alg

#endif // ALGEBRA_CORE_TYPES_DYNAMIC_VECTOR_HH
