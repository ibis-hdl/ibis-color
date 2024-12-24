//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <ibis/color/detail/attribute_container.hpp>

#include <algorithm>
#include <iostream>

namespace ibis::color {

namespace detail {

class ansi_writer {
public:
    explicit ansi_writer(std::ostream& os_)
        : os{ os_ }
    {
    }

public:
    template <typename enum_type, std::size_t SIZE>
    void operator()(detail::attribute_container<enum_type, SIZE> const& attributes) const
    {
        if (attributes.count() == 0) {
            return;
        }

        char const* delimiter = "";

        os << CSI;
        for (std::size_t i = 0; i != attributes.count(); ++i) {
            // don't interpret as char_type
            os << delimiter << static_cast<short>(attributes[i]);
            delimiter = ";";
        }
        os << "m";
    }

    std::ostream& os;
    /// ANSI Control Sequence Introducer/Initiator
    static constexpr char CSI[] = "\x1B[";
};

}  // namespace detail

}  // namespace ibis::color
