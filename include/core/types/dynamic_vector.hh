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

#include <core/types/ring.hh>
#include <core/types/field.hh>

#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <new>
#include <ranges>
#include <span>
#include <utility>

namespace alg {

template <ring Ring>
class dynamic_vector {
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
        m_values = (Ring*) std::assume_aligned<alignof(Ring)>(allocate_uninitialized(other.size()));
        m_end = m_values + other.size();

        std::uninitialized_move(other.m_values, other.m_end, m_values);
    }

    dynamic_vector& operator=(dynamic_vector&& other) {
        if (this != std::addressof(other)) {
            std::destroy_at(this);
            std::construct_at(this, std::move(other));
        }
        return *this;
    }

    dynamic_vector(std::size_t size, const Ring& init_value) {
        assert(size > 0);
        m_values = (Ring*) std::assume_aligned<alignof(Ring)>(allocate_uninitialized(size));
        m_end = m_values + size;

        std::uninitialized_fill(m_values, m_end, init_value);
    }

    template <std::forward_iterator It>
        requires std::convertible_to<std::iter_value_t<It>, Ring>
    dynamic_vector(It begin, It end) {
        const auto size = std::distance(begin, end);
        assert(begin != end);

        m_values = (Ring*) std::assume_aligned<alignof(Ring)>(allocate_uninitialized(size));
        m_end = m_values;

        std::size_t idx = 0;
        for (It it = begin; it != end; ++it) {
            std::construct_at(m_values + idx, *it);
            ++m_end;
        }
    }

    template <std::ranges::range Range>
        requires std::convertible_to<std::ranges::range_value_t<Range>, Ring>
    dynamic_vector(Range&& range)
        : dynamic_vector(std::ranges::begin(range), std::ranges::end(range))
    {}

    ~dynamic_vector() noexcept {
        std::destroy(m_values, m_end);
        std::free(m_values);
    }

public:
    auto&& operator[](this auto&& self, std::size_t idx) noexcept {
        Ring* vals = std::assume_aligned<alignof(Ring)>(m_values);
        return vals[idx];
    }

    [[gnu::const]] std::size_t size() const noexcept {
        return m_end - m_values;
    }

    /// dynamic_vector-vector operations.
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
        return define_scalar_op(r, *this, [] (const Ring& lhs, const Ring& rhs) { return lhs + rhs; });
    }
    dynamic_vector& operator+=(const Ring& r) {
        return define_scalar_op_self(r, *this, [] (const Ring& lhs, Ring& rhs) { rhs += lhs; });
    }
    friend dynamic_vector operator+(const Ring& r, const dynamic_vector& vec) {
        return vec + r;
    }

    dynamic_vector operator-(const Ring& r) const {
        return define_scalar_op(r, *this, [] (const Ring& lhs, const Ring& rhs) { return lhs - rhs; });
    }
    dynamic_vector& operator-=(const Ring& r) {
        return define_scalar_op_self(r, *this, [] (const Ring& lhs, Ring& rhs) { rhs -= lhs; });
    }
    friend dynamic_vector operator+(const Ring& r, const dynamic_vector& vec) {
        return vec - r;
    }

    dynamic_vector operator*(const Ring& r) const {
        return define_scalar_op(r, *this, [] (const Ring& lhs, const Ring& rhs) { return lhs * rhs; });
    }
    dynamic_vector& operator*=(const Ring& r) {
        return define_scalar_op_self(r, *this, [] (const Ring& lhs, Ring& rhs) { rhs *= lhs; });
    }
    friend dynamic_vector operator*(const Ring& r, const dynamic_vector& vec) {
        return vec * r;
    }

    template <typename = typename std::enable_if<field<Ring>>::type>
    dynamic_vector operator/(const Ring& r) const {
        return define_scalar_op(r, *this, [] (const Ring& lhs, const Ring& rhs) { return rhs / lhs; });
    }
    template <typename = typename std::enable_if<field<Ring>>::type>
    dynamic_vector& operator/=(const Ring& r) const {
        return define_scalar_op_self(r, *this, [] (const Ring& lhs, Ring& rhs) { rhs /= lhs; });
    }

    /// Miscellaneous.
    [[gnu::const]] Ring dot(const dynamic_vector& other) const {
        const auto size = this->size();
        assert(size == other.size());
        [[assume(size == other.size())]];

        Ring* ldata = std::assume_aligned<alignof(Ring)>(m_values);
        Ring* rdata = std::assume_aligned<alignof(Ring)>(other.m_values);

        Ring result = ring_zero<Ring>();
        for (std::size_t idx = 0; idx < size; ++idx) {
            result += ldata[idx] * rdata[idx];
        }
        return result;
    }

public:
    static dynamic_vector zero(std::size_t size) {
        assert(size > 0);
        return dynamic_vector(size, ring_zero<Ring>());
    }
    static dynamic_vector one(std::size_t size) {
        assert(size > 0);
        return dynamic_vector(size, ring_zero<Ring>());
    }

private:
    static Ring* allocate_uninitialized(std::size_t size) {
        Ring* result = (Ring*) std::aligned_alloc(alignof(Ring), sizeof(Ring) * size);
        if (result == nullptr) {
            throw std::bad_alloc{};
        }
        return result;
    }

    template <typename Op>
    static dynamic_vector define_vec_op(const dynamic_vector& lhs, const dynamic_vector& rhs, const Op& op) {
        assert(lhs.size() == rhs.size());
        [[assume(lhs.size() == rhs.size())]];
        
        const auto size = lhs.size();
        Ring* data = std::assume_aligned<alignof(Ring)>(allocate_uninitialized(size));
        std::size_t idx = 0;

        try {
            for (; idx < size; ++idx) {
                std::construct_at(data + idx, op(lhs, rhs));
            }
        } catch (...) {
            for (std::size_t i = 0; i < idx; ++i) {
                std::destroy_at(data + i);
            }
            std::free(data);
        }

        return dynamic_vector(data, data + size, private_constructor_marker{});
    }

    template <typename Op>
    static dynamic_vector& define_vec_op_self(dynamic_vector& lhs, const dynamic_vector& rhs, const Op& op) {
        const auto size = lhs.size();
        assert(size == rhs.size());
        [[assume(size == rhs.size())]];

        Ring* ldata = std::assume_aligned<alignof(Ring)>(lhs.m_values);
        Ring* rdata = std::assume_aligned<alignof(Ring)>(rhs.m_values);

        for (std::size_t i = 0; i < size; ++i) {
            op(ldata[i], rdata[i]);
        }

        return lhs;
    }

    template <typename Op>
    static dynamic_vector define_scalar_op(const Ring& r, const dynamic_vector& vec, const Op& op) {
        const auto size = vec.size();
        Ring* data = std::assume_aligned<alignof(Ring)>(allocate_uninitialized(size));
        std::size_t idx = 0;

        try {
            for (; idx < size; ++idx) {
                std::construct_at(data + idx, op(r, vec.m_values[idx]));
            }
        } catch (...) {
            for (std::size_t i = 0; i < idx; ++i) {
                std::destroy_at(data + i);
            }
            std::free(data);
        }

        return dynamic_vector(data, data + size, private_constructor_marker{});
    }

    template <typename Op>
    static dynamic_vector& define_scalar_op_self(const Ring& r, dynamic_vector& vec, const Op& op) {
        const auto size = vec.size();
        Ring* data = std::assume_aligned<alignof(Ring)>(vec.m_values);

        for (std::size_t i = 0; i < size; ++i) {
            op(r, data[i]);
        }
        return vec;
    }
};

} // namespace alg

#endif // ALGEBRA_CORE_TYPES_DYNAMIC_VECTOR_HH
