//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <windows.h>
#include <io.h>  // _isatty, _fileno

#include <iostream>

namespace ibis::color::platform_windows {

class isatty {
    bool is_tty(std::streambuf const* rdbuf) const noexcept
    {
        if (rdbuf == std::cout.rdbuf()) {
            return _isatty(_fileno(stdout));
        }
        if (rdbuf == std::cerr.rdbuf() || rdbuf == std::clog.rdbuf()) {
            return _isatty(_fileno(stderr));
        }
        return false;
    }

public:
    isatty(std::ostream const& os)
        : tty{ is_tty(os.rdbuf()) }
    {
    }

    constexpr operator bool() const { return tty; }

private:
    bool const tty;
};

class stream_handle {
public:
    stream_handle(std::ostream const& os)
        : hConsoleHandle{ get_handle(os.rdbuf()) }
    {
    }

    stream_handle(std::streambuf const* rdbuf)
        : hConsoleHandle{ get_handle(rdbuf) }
    {
    }

    stream_handle(stream_handle&) = delete;
    stream_handle& operator=(stream_handle&) = delete;

public:
    constexpr operator HANDLE() const { return hConsoleHandle; }

private:
    HANDLE get_handle(std::streambuf const* rdbuf) const noexcept
    {
        if (rdbuf == std::cout.rdbuf()) {
            return GetStdHandle(STD_OUTPUT_HANDLE);
        }

        if (rdbuf == std::cerr.rdbuf() || rdbuf == std::clog.rdbuf()) {
            return GetStdHandle(STD_ERROR_HANDLE);
        }

        return INVALID_HANDLE_VALUE;
    }

private:
    HANDLE const hConsoleHandle;
};

class ansi_color_support {
public:
    ansi_color_support(std::ostream const& os)
        : support{ has_support(os) }
    {
    }

    constexpr operator bool() const { return support; }

private:
    bool set_ansi(std::streambuf const* rdbuf) const noexcept
    {
        HANDLE hConsoleHandle = stream_handle(rdbuf);

        if (hConsoleHandle == INVALID_HANDLE_VALUE) {
            return false;
        }

        /* Check for [Console Virtual Terminal Sequences](
         * https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences)
         * capabilities. */
        DWORD lpMode = 0;

        if (!GetConsoleMode(hConsoleHandle, &lpMode)) {
            return false;
        }

        lpMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

        if (!SetConsoleMode(hConsoleHandle, lpMode)) {
            return false;
        }

        return true;
    }

    bool has_support(std::ostream const& os) const noexcept
    {
        if (os.rdbuf() == std::cerr.rdbuf() || os.rdbuf() == std::cout.rdbuf()) {
            return set_ansi(os.rdbuf());
        }

        return false;
    }

private:
    bool const support;
};

}  // namespace ibis::color::platform_windows
