//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <iosfwd>
#include <array>
#include <iterator>
#include <algorithm>
#include <cassert>

namespace ibis::color {

namespace detail {

template <typename attribute_type, std::size_t SIZE>
class attribute_container
    : public std::array<typename std::underlying_type<attribute_type>::type, SIZE> {
public:
    using value_type = typename std::underlying_type<attribute_type>::type;

private:
    template <std::size_t... N>
    explicit attribute_container(std::initializer_list<attribute_type> il,
                                 std::index_sequence<N...>)
        : std::array<value_type, SIZE>{}  // default initialized (with 0x00)
        , idx{ il.size() }
    {
        assert(il.size() <= SIZE && "range error of initializer_list");

        std::transform(il.begin(), il.begin() + std::min(SIZE, il.size()), this->begin(),
                       [](attribute_type x) { return static_cast<value_type>(x); });
    }

public:
    explicit attribute_container(std::initializer_list<attribute_type> il)
        : attribute_container(il, std::make_index_sequence<SIZE>{})
    {
    }

    attribute_container() = default;
    ~attribute_container() = default;

    attribute_container(attribute_container const&) = default;
    attribute_container& operator=(attribute_container const&) = default;

    attribute_container(attribute_container&&) = default;
    attribute_container& operator=(attribute_container&&) = default;

public:
    void push_back(attribute_type code)
    {
        assert(idx < size && "Code array is full");
        (*this)[idx++] = static_cast<value_type>(code);
    }

    attribute_container& operator|=(attribute_container const& other)
    {
        assert((idx + other.idx) <= size && "Merged size to big");

        std::copy_n(other.begin(), other.idx, &(*this)[idx]);
        idx += other.idx;

        return *this;
    }

    attribute_container& operator|=(attribute_type code)
    {
        push_back(code);
        return *this;
    }

    std::size_t count() const { return idx; }

    void reset()
    {
        (*this).fill(0);
        idx = 0;
    }

public:
    static constexpr std::size_t size = SIZE;

private:
    std::size_t idx = 0;
};

template <typename attribute_type, std::size_t SIZE>
inline attribute_container<attribute_type, SIZE> operator|(
    attribute_container<attribute_type, SIZE> lhs,
    attribute_container<attribute_type, SIZE> const& rhs)
{
    lhs |= rhs;
    return lhs;
}

}  // namespace detail

}  // namespace ibis::color
