//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <tuple>
#include <type_traits>
#include <iosfwd>

namespace ibis::color {

template <typename EnumT>
class basic_manip {
public:
    explicit basic_manip(EnumT value_)
        : enum_value{ value_ }
    {
    }

    /// allocates the iword storage for use with manipulator objects
    static int32_t xindex()
    {
        static int idx = std::ios_base::xalloc();
        return idx;
    }

    /// use stream's private storage to store the manipulator value
    void set_value(std::ostream& os) const { os.iword(xindex()) = static_cast<long>(enum_value); }

    /// return the flag about
    static EnumT value(std::ostream& os) { return static_cast<EnumT>(os.iword(xindex())); }

private:
    EnumT enum_value;
};

/// required to apply the Stream Manipulator
template <typename EnumT>
std::ostream& operator<<(std::ostream& os, basic_manip<EnumT> const& manip)
{
    manip.set_value(os);
    return os;
}

/// Behavior
enum class control {
    Auto = 0,  ///< auto detect terminal and colorize if needed (Default)
    Off = 1,   ///< toggle off
    Force = 2  ///< force ANSI color output to non terminal streams
};

/// Windows only: Terminal Mode, no effect on UNIX
enum class terminal {
    Auto = 0,   ///< automatically detects whether ANSI or Native API (Default)
    Ansi = 1,   ///< Force use ANSI API
    Native = 2  ///< Force use Native API
};

/// The Stream Manipulator for controlling the output
template <typename EnumT>
typename std::enable_if_t<  // --
    std::is_same_v<EnumT, control> || std::is_same_v<EnumT, terminal>, basic_manip<EnumT> >
apply(EnumT modifier)
{
    return basic_manip<EnumT>{ modifier };
}

}  // namespace ibis::color
