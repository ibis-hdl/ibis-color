//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <ibis/color/detail/attribute_container.hpp>
#include <ibis/color/detail/platform.hpp>

#include <ibis/color/attribute.hpp>

namespace ibis::color {

namespace detail {

class winapi_writer {
public:
    explicit winapi_writer(std::ostream& os_, int32_t xindex_)
        : os{ os_ }
        , hConsoleHandle{ platform_windows::stream_handle{ os } }
        , xindex{ xindex_ }
    {
    }

public:
    template <typename enum_type, std::size_t SIZE>
    void operator()(detail::attribute_container<enum_type, SIZE> const& attributes) const
    {
        if (attributes.count() == 0u) {
            return;
        }

        // os << "WinAPI: ";

        // store the data required for prefix/postfix printing persistent into
        // std::ios_base's private storage.
        long iword_data = os.iword(xindex);
        iword_data = fsm(iword_data, native(attributes));
        os.iword(xindex) = iword_data;
    }

private:
    ///
    /// The FSM is used to restore stream's color state after facet's message is
    /// colorized written to it's original state.
    ///
    /// \note The C99 approach of using unions for type pruning doesn't work on
    /// C++ due to violating C/C++'s 'strict aliasing rule which basically says that if you cast a
    /// pointer to a different kind of pointer and then dereference it, then your program has
    /// executed an undefined behavior, see e.g.  C++CoreGuidelines
    /// [C.183: Don't use a union for typepruning](
    ///  http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ru-pun) for
    /// details. Here the memcpy() solution is used as described at [Type Pruning, Strict Aliasing,
    /// and Optimization](https://blog.regehr.org/archives/959).
    ///
    long fsm(long iword, WORD attr) const
    {
        typedef enum : uint8_t { init, active, passiv } state_t;

        struct {
            state_t state;
            WORD saved_attribute;
        } data;

        static_assert(sizeof(data) <= sizeof(long), "iword storage size to big");

        memcpy(&data, &iword, sizeof(long));

        state_t next_state{ init };

        // clang-format off
        switch (data.state) {
            case init:
                // store attributes to be able to restore them later on
                data.saved_attribute = console_attributes();
                [[fallthrough]];
            case active: {
                    // check on unsupported text-only attribute
                    if ((attr & 0xFF) == 0) {
                        // attr = 0 means no visibility
                        attr = data.saved_attribute;
                    }
                    // set only color relevant (lower) byte
                    WORD const upper = (data.saved_attribute >> 8) & 0xFF;
                    // tamming C integer promotion to avoid warning [-Wimplicit-int-conversion]
                    attr = static_cast<WORD>((upper << 8) | (attr & 0xFF));

                    SetConsoleTextAttribute(hConsoleHandle, attr);
                    next_state = passiv;
                }
                break;
            case passiv: {
                    // restore attributes
                    SetConsoleTextAttribute(hConsoleHandle, data.saved_attribute);
                    next_state = active;
                }
                break;
            default:
                // no abort() call here due to low severity of the error
                std::cerr << __func__ << ": INVALID STATE\n";
                next_state = init;
        }
        // clang-format on

        data.state = next_state;

        memcpy(&iword, &data, sizeof(long));

        return iword;
    }

    WORD console_attributes() const
    {
        if (hConsoleHandle == INVALID_HANDLE_VALUE) {
            return 0;
        }

        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        if (!GetConsoleScreenBufferInfo(hConsoleHandle, &csbi)) {
            return 0;
        }

        return csbi.wAttributes;
    }

    ///
    /// Convert portable color codes to windows console screen buffer specified
    /// values.
    ///
    /// foreground color: (0-15) lower 3 RGB bits + intensity bit
    /// background color: (0-15) higher 3 RGB bits + intensity bit
    /// bold: emulated as FOREGROUND_INTENSITY bit
    /// underline:  emulated as BACKGROUND_INTENSITY bit
    /// inverse: swap foreground/bold & background/underline
    /// conceal: set foreground/bold to background/underline
    ///
    /// \see [Console Screen
    /// Buffers](https://docs.microsoft.com/en-us/windows/console/console-screen-buffers)
    ///
    /// @param attributes Portable color attributes.
    /// @return Win API color code
    ///
    template <typename enum_type, std::size_t SIZE>
    WORD native(detail::attribute_container<enum_type, SIZE> const& attributes) const
    {
        auto const win_attribute = [](color::attribute attr_) -> WORD {
            switch (attr_) {
                // --- Style ---
                case color::attribute::Attributes_Off:
                    return 0x000;
                case color::attribute::Text_Bold:
                    return 0x100;
                case color::attribute::Text_Underscore:
                    return 0x200;
                case color::attribute::Text_Blink:
                    return 0x400;
                case color::attribute::Text_Reverse:
                    return 0x800;
                // --- Foreground Color ---
                case color::attribute::Foreground_Black:
                    // 0x0000
                    return 0;
                case color::attribute::Foreground_Red:
                    // 0x0004
                    return FOREGROUND_RED;
                case color::attribute::Foreground_Green:
                    // 0x0002
                    return FOREGROUND_GREEN;
                case color::attribute::Foreground_Yellow:
                    // 0x0006
                    return FOREGROUND_GREEN | FOREGROUND_RED;
                case color::attribute::Foreground_Blue:
                    // 0x0001
                    return FOREGROUND_BLUE;
                case color::attribute::Foreground_Magenta:
                    // 0x0005
                    return FOREGROUND_BLUE | FOREGROUND_RED;
                case color::attribute::Foreground_Cyan:
                    // 0x0003
                    return FOREGROUND_BLUE | FOREGROUND_GREEN;
                    // 0x0003
                case color::attribute::Foreground_White:
                    // 0x0007 - aka grey
                    return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                // --- Background Color ---
                case color::attribute::Background_Black:
                    // 0x0000
                    return 0;
                case color::attribute::Background_Red:
                    // 0x0040
                    return BACKGROUND_RED;
                case color::attribute::Background_Green:
                    // 0x0020
                    return BACKGROUND_GREEN;
                case color::attribute::Background_Yellow:
                    // 0x0060
                    return BACKGROUND_GREEN | BACKGROUND_RED;
                case color::attribute::Background_Blue:
                    // 0x0010
                    return BACKGROUND_BLUE;
                case color::attribute::Background_Magenta:
                    // 0x0050
                    return BACKGROUND_BLUE | BACKGROUND_RED;
                case color::attribute::Background_Cyan:
                    // 0x0030
                    return BACKGROUND_BLUE | BACKGROUND_GREEN;
                case color::attribute::Background_White:
                    // 0x0070 - aka grey
                    return BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
                default:
                    // no abort() call here due to low severity of the error
                    std::cerr << __func__ << ": INVALID STATE\n";
                    return 0;
            }
        };

        WORD attr = 0;

        for (std::size_t i = 0; i != attributes.count(); ++i) {
            // XXX handle bold, inverse, underline etc.
            attr |= win_attribute(static_cast<color::attribute>(attributes[i]));
        }

        return attr;
    }

private:
    std::ostream& os;
    HANDLE const hConsoleHandle;
    int32_t const xindex;
};

}  // namespace detail

}  // namespace ibis::color
