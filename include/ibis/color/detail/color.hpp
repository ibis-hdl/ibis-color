//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <ibis/util/platform.hpp>

#if defined(IBIS_BUILD_PLATFORM_WINDOWS)
// not fully C++17 MinGW g++-7.3.0
// #  include <ibis/util/compiler/detail/compiler_push.hpp>
// #  if defined(__GNUC__) && !defined(__clang__)
// #    pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
// #  endif
#include <ibis/color/detail/winapi_printer.hpp>
// #  include <ibis/util/compiler/detail/compiler_pop.hpp>
#else
#include <ibis/color/detail/ansi_esc_printer.hpp>
#endif
