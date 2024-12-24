//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <cstdio>    // fileno
#include <unistd.h>  // isatty

#include <iostream>

namespace ibis::color::platform_unix {

class isatty {
public:
    isatty(std::ostream const& os)
        : tty{ is_tty(os.rdbuf()) }
    {
    }

    constexpr operator bool() const { return tty; }

private:
    bool is_tty(std::streambuf const* rdbuf) const noexcept
    {
        // ordered by likelihood
        if (rdbuf == std::cout.rdbuf()) {
            return ::isatty(::fileno(stdout));
        }
        if (rdbuf == std::cerr.rdbuf() || rdbuf == std::clog.rdbuf()) {
            return ::isatty(::fileno(stderr));
        }
        return false;
    }

private:
    bool const tty;
};

}  // namespace ibis::color::platform_unix
