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
#include <volplay/sdf_plane.h>
#include <volplay/sdf_displacement.h>
#include <volplay/sdf_make.h>

namespace vp = volplay;

TEST_CASE("SDfDisplacement")
{
    vp::SDFNodePtr scene = vp::make()
        .displacement().fnc([](const vp::Vector &v) -> vp::S { return 1;})
            .plane().normal(vp::Vector::UnitZ())
        .end();

    REQUIRE_CLOSE( scene->eval(vp::Vector(0,0,0)), vp::S(1) );
    REQUIRE_CLOSE( scene->eval(vp::Vector(1,0,0)), vp::S(1) );
    REQUIRE_CLOSE( scene->eval(vp::Vector(0,1,0)), vp::S(1) );
    REQUIRE_CLOSE( scene->eval(vp::Vector(1,0,1)), vp::S(2) );
    REQUIRE_CLOSE( scene->eval(vp::Vector(0,0,-1)), vp::S(0) );
    REQUIRE_CLOSE( scene->eval(vp::Vector(10,10,10)), vp::S(11) );
    
}