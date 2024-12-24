//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <ibis/color/attribute.hpp>
#include <ibis/color/detail/ansi_writer.hpp>
#include <ibis/color/manip.hpp>

#include <ibis/color/detail/winapi_writer.hpp>
#include <ibis/color/detail/platform.hpp>

#include <iostream>

namespace ibis::color {

namespace detail {

template <typename enum_type, std::size_t SIZE>
class winapi_printer {
public:
    using attribute = detail::attribute_container<enum_type, SIZE>;

public:
    explicit winapi_printer(attribute const attributes_)
        : attributes{ attributes_ }
    {
    }

    winapi_printer() = default;

public:
    std::ostream& print_on(std::ostream& os) const
    {
        auto const use_ansi_writer = [&](color::terminal term_mode) -> bool {
            switch (term_mode) {
                case color::terminal::Auto:
                    return platform_windows::ansi_color_support{ os };
                case color::terminal::Ansi:
                    return true;
                default:  // aka color::terminal::Native
                    return false;
            }
        };

        color::terminal const term = basic_manip<color::terminal>::value(os);

        // first use initialization
        if (!use_ansi) {
            *use_ansi = use_ansi_writer(term);
        }

        // only on changes check which concrete writer implementation to use
        if (term != terminal_mode) {
            terminal_mode = term;
            *use_ansi = use_ansi_writer(term);
        }

        if (*use_ansi) {
            print_ansi(os);
        }
        else {
            print_native(os);
        }

        return os;
    }

private:
    void print_ansi(std::ostream& os) const
    {
        detail::ansi_writer ansi{ os };
        ansi(attributes);
    }

    void print_native(std::ostream& os) const
    {
        detail::winapi_writer writer{ os, xindex };
        writer(attributes);
    }

private:
    attribute const attributes;
    static const int xindex;
    color::terminal mutable terminal_mode;
    std::optional<bool> mutable use_ansi;
};

template <typename enum_type, std::size_t SIZE>
inline std::ostream& operator<<(  // --
    std::ostream& os, winapi_printer<enum_type, SIZE> const& printer)
{
    return printer.print_on(os);
}

}  // namespace detail

}  // namespace ibis::color
