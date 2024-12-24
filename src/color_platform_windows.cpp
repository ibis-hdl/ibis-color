//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#include <ibis/color/detail/color.hpp>  // IWYU pragma: keep

namespace ibis::color::detail {

template <>
const int32_t winapi_printer<color::attribute, 4>::xindex = std::ios_base::xalloc();

}
