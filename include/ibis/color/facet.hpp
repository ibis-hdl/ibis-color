//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <ibis/color/attribute.hpp>
#include <ibis/color/manip.hpp>
#include <ibis/color/detail/color.hpp>
#include <ibis/color/detail/platform.hpp>
#include <ibis/util/platform.hpp>

#include <iostream>
#include <locale>  // facet
#include <optional>

namespace ibis::color {

template <typename Tag>
class message_facet : public std::locale::facet {
public:
    using attributes = detail::attribute_container<color::attribute, 4>;

public:
    ///
    /// Construct a message facet
    ///
    /// @param prefix_     The color before printing the embraced message.
    /// @param postfix_    The color past printing the embraced message, commonly
    ///                   restore the default state.
    ///
    /// \note Clang-Tidy '[misc-move-const-arg]' message:
    /// std::move of the variable {'prefix_', 'postfix_'} of the
    /// trivially-copyable type 'attribute_container<color::attribute, 4>'
    /// has no effect.
    ///
    explicit message_facet(attributes prefix_, attributes postfix_)
        : facet{ 0 }
        , prefix{ prefix_ }
        , postfix{ postfix_ }
    {
    }

    explicit message_facet()
        : facet{ 0 }
    {
    }

    ~message_facet() override = default;

    message_facet(message_facet const&) = delete;
    message_facet& operator=(message_facet const&) = delete;

    message_facet(message_facet&&) = delete;
    message_facet& operator=(message_facet&&) = delete;

public:
    std::ostream& print_on(std::ostream& os, message_decorator<Tag> const& message) const
    {
#if defined(IBIS_BUILD_PLATFORM_WINDOWS)
        using printer = detail::winapi_printer<color::attribute, 4>;
#else
        using printer = detail::ansi_esc_printer<color::attribute, 4>;
#endif

        auto const use_color = [&](color::control ctrl) -> std::optional<bool> {
            switch (ctrl) {
                case color::control::Auto:
                    return detail::isatty{ os };
                case color::control::Force:
                    return true;
                default:  // aka color::control::Off
                    return false;
            }
        };

        auto const ctrl = basic_manip<color::control>::value(os);

        // first time initialization
        if (!enable) {
            enable = use_color(ctrl);
        }

        // only on changes check to use colors
        if (ctrl != control) {
            control = ctrl;
            enable = use_color(ctrl);
        }

        if (*enable) {
            printer prefix_{ prefix };
            os << prefix_;
        }

        message.print_on(os);

        if (*enable) {
            printer postfix_{ postfix };
            os << postfix_;
        }

        return os;
    }

public:
    attributes prefix;
    attributes postfix;

public:
    static std::locale::id id;

private:
    color::control mutable control;
    std::optional<bool> mutable enable;
};

template <typename Tag>
std::locale::id message_facet<Tag>::id;

template <typename Tag>
std::ostream& operator<<(std::ostream& os, message_decorator<Tag> const& decorator)
{
    std::locale locale = os.getloc();

    if (std::has_facet<message_facet<Tag>>(locale)) {
        return std::use_facet<message_facet<Tag>>(locale).print_on(os, decorator);
    }

    return decorator.print_on(os);
}

}  // namespace ibis::color
