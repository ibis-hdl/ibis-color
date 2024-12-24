//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <ibis/color/detail/attribute_container.hpp>

#include <cstdint>
#include <string_view>
#include <optional>

namespace ibis::color {

enum class attribute : uint8_t {
    // Text attribute
    Attributes_Off = 0,
    Text_Bold = 1,
    Text_Underscore = 4,
    Text_Blink = 5,
    Text_Reverse = 7,
    Text_Concealed = 8,
    // Foreground colors
    Foreground_Black = 30,
    Foreground_Red = 31,
    Foreground_Green = 32,
    Foreground_Yellow = 33,
    Foreground_Blue = 34,
    Foreground_Magenta = 35,
    Foreground_Cyan = 36,
    Foreground_White = 37,
    // Background colors
    Background_Black = 40,
    Background_Red = 41,
    Background_Green = 42,
    Background_Yellow = 43,
    Background_Blue = 44,
    Background_Magenta = 45,
    Background_Cyan = 46,
    Background_White = 47,
};

using attribute_container = detail::attribute_container<color::attribute, 4>;

namespace text {
attribute_container const bold{ attribute::Text_Bold };
attribute_container const underscore{ attribute::Text_Underscore };
}  // namespace text

namespace fg {
attribute_container const black{ attribute::Foreground_Black };
attribute_container const red{ attribute::Foreground_Red };
attribute_container const green{ attribute::Foreground_Green };
attribute_container const yellow{ attribute::Foreground_Yellow };
attribute_container const blue{ attribute::Foreground_Blue };
attribute_container const magenta{ attribute::Foreground_Magenta };
attribute_container const cyan{ attribute::Foreground_Cyan };
attribute_container const white{ attribute::Foreground_White };
}  // namespace fg

namespace bg {
attribute_container const black{ attribute::Background_Black };
attribute_container const red{ attribute::Background_Red };
attribute_container const green{ attribute::Background_Green };
attribute_container const yellow{ attribute::Background_Yellow };
attribute_container const blue{ attribute::Background_Blue };
attribute_container const magenta{ attribute::Background_Magenta };
attribute_container const cyan{ attribute::Background_Cyan };
attribute_container const white{ attribute::Background_White };
}  // namespace bg

attribute_container const color_off{ attribute::Attributes_Off };

std::optional<color::attribute> text_attr(std::string_view name);
std::optional<color::attribute> foreground_attr(std::string_view name);
std::optional<color::attribute> background_attr(std::string_view name);

}  // namespace ibis::color
