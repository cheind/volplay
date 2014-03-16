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
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_repetition.h>

namespace vp = volplay;

TEST_CASE("SDFRepetition")
{
    vp::SDFRepetition r(vp::Vector(5, 5, 5), std::make_shared<vp::SDFSphere>());
    
    REQUIRE_CLOSE( r.eval(vp::Vector(0,0,0)), vp::S(-1) );
    REQUIRE_CLOSE( r.eval(vp::Vector(5,5,5)), vp::S(-1) );
    REQUIRE_CLOSE( r.eval(vp::Vector(-5,-5,-5)), vp::S(-1) );
    REQUIRE_CLOSE( r.eval(vp::Vector(1,0,0)), vp::S(0) );
    REQUIRE_CLOSE( r.eval(vp::Vector(6,0,0)), vp::S(0) );
    

}