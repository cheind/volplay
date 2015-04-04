// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include "catch.hpp"
#include "float_comparison.hpp"
#include <volplay/math/root.h>

namespace vp = volplay;

TEST_CASE("Roots")
{
    // Function: https://www.wolframalpha.com/input/?i=+x%5E3+-+6x%5E2+%2B+4x+%2B+12
    auto f = [](vp::S x) { return std::pow(x, 3) - 6 * std::pow(x, 2) + 4 * x + 12; };
    
    vp::S r;
    REQUIRE( vp::math::findRootBrent(f, vp::S(-1.1), vp::S(-1.0), vp::S(0.0001), 20, r));
    REQUIRE_CLOSE_PREC(vp::S(-1.0514), r, vp::S(0.0001));
    REQUIRE( vp::math::findRootBrent(f, vp::S(2.4), vp::S(2.6), vp::S(0.0001), 20, r));
    REQUIRE_CLOSE_PREC(vp::S(2.5173), r, vp::S(0.0001));
    REQUIRE( vp::math::findRootBrent(f, vp::S(4), vp::S(10), vp::S(0.0001), 20, r));
    REQUIRE_CLOSE_PREC(vp::S(4.5341), r, vp::S(0.0001));
    
    REQUIRE(!vp::math::findRootBrent(f, vp::S(10), vp::S(12), vp::S(0.0001), 20, r));
    
}