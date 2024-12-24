//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <ibis/util/platform.hpp>

#if defined(IBIS_BUILD_PLATFORM_WINDOWS)
#include <ibis/color/detail/platform_windows.hpp>
#else
#include <ibis/color/detail/platform_unix.hpp>
#endif

namespace ibis::color::detail {

#if defined(IBIS_BUILD_PLATFORM_WINDOWS)
using platform_windows::isatty;
#else
using platform_unix::isatty;
#endif

}  // namespace ibis::color::detail
