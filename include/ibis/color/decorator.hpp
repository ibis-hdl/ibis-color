//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <iosfwd>
#include <string>

namespace ibis::color {

template <typename Tag>
class message_decorator {
public:
    /// @todo [Boost.Locale future] must be const reference to std::string due to boost.locale API
    /// which returns basic_message<char> which isn't convertible from 'basic_message<char>' to
    /// 'std::string_view' yet, so we stick std::basic_string<char_type>
    explicit message_decorator(std::string const& contents_)
        : contents{ contents_ }
    {
    }

    std::ostream& print_on(std::ostream& os) const
    {
        os << contents;
        return os;
    }

private:
    std::string const& contents;
};

}  // namespace ibis::color
