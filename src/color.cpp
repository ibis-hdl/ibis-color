//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#include <ibis/color/message.hpp>

#include <ibis/util/icompare.hpp>

#include <initializer_list>
#include <locale>
#include <optional>
#include <string_view>
#include <utility>

namespace ibis::color {

// static std::locale::id for each facet
template <>
std::locale::id message_facet<
    message::severity::failure>::id;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
template <>
std::locale::id message_facet<
    message::severity::error>::id;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
template <>
std::locale::id message_facet<
    message::severity::warning>::id;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
template <>
std::locale::id message_facet<
    message::severity::note>::id;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

std::optional<color::attribute> text_attr(std::string_view name)
{
    using namespace std::literals::string_view_literals;

    static std::initializer_list<std::pair<std::string_view, color::attribute>> constexpr map{
        { "bold"sv, color::attribute::Text_Bold }
    };

    for (auto const& [color_name, attribute] : map) {
        if (util::icompare(color_name, name)) {
            return { attribute };
        }
    }

    return std::optional<color::attribute>{};
}

std::optional<color::attribute> foreground_attr(std::string_view name)
{
    using namespace std::literals::string_view_literals;

    static std::initializer_list<std::pair<std::string_view, color::attribute>> constexpr map{
        { "black"sv, color::attribute::Foreground_Black },
        { "red"sv, color::attribute::Foreground_Red },
        { "green"sv, color::attribute::Foreground_Green },
        { "yellow"sv, color::attribute::Foreground_Yellow },
        { "blue"sv, color::attribute::Foreground_Blue },
        { "magenta"sv, color::attribute::Foreground_Magenta },
        { "cyan"sv, color::attribute::Foreground_Cyan },
        { "white"sv, color::attribute::Foreground_White }
    };

    for (auto const& [color_name, attribute] : map) {
        if (util::icompare(color_name, name)) {
            return { attribute };
        }
    }

    return std::optional<color::attribute>{};
}

std::optional<color::attribute> background_attr(std::string_view name)
{
    using namespace std::literals::string_view_literals;

    static std::initializer_list<std::pair<std::string_view, color::attribute>> constexpr map{
        { "black"sv, color::attribute::Background_Black },
        { "red"sv, color::attribute::Background_Red },
        { "green"sv, color::attribute::Background_Green },
        { "yellow"sv, color::attribute::Background_Yellow },
        { "blue"sv, color::attribute::Background_Blue },
        { "magenta"sv, color::attribute::Background_Magenta },
        { "cyan"sv, color::attribute::Background_Cyan },
        { "white"sv, color::attribute::Background_White }
    };

    for (auto const& [color_name, attribute] : map) {
        if (util::icompare(color_name, name)) {
            return { attribute };
        }
    }

    return {};
}

}  // namespace ibis::color
