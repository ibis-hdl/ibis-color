//
// Copyright (c) 2017-2022 Olaf (<ibis-hdl@users.noreply.github.com>).
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <ibis/color/detail/color.hpp>

#include <ibis/color/decorator.hpp>
#include <ibis/color/facet.hpp>

namespace ibis::color::message {

typedef struct {  // VHDL severities
    struct note;
    struct warning;
    struct error;
    struct failure;
} severity;

using note = message_decorator<severity::note>;
using warning = message_decorator<severity::warning>;
using error = message_decorator<severity::error>;
using failure = message_decorator<severity::failure>;

using note_facet = message_facet<severity::note>;
using warning_facet = message_facet<severity::warning>;
using error_facet = message_facet<severity::error>;
using failure_facet = message_facet<severity::failure>;

}  // namespace ibis::color::message
