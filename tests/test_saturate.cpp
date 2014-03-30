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
#include <volplay/rendering/saturate.h>

namespace vpr = volplay::rendering;

TEST_CASE("Saturate")
{
    REQUIRE(vpr::saturate<int>(10) == 10);
    REQUIRE(vpr::saturate<int>(-10) == -10);
    
    REQUIRE(vpr::saturate<char>(127) == 127);
    REQUIRE(vpr::saturate<char>(128) == 127);
    REQUIRE(vpr::saturate<char>(-128) == -128);
    
    REQUIRE(vpr::saturate<unsigned char>(127) == 127);
    REQUIRE(vpr::saturate<unsigned char>(128) == 128);
    REQUIRE(vpr::saturate<unsigned char>(255) == 255);
    REQUIRE(vpr::saturate<unsigned char>(-128) == 0);
}

