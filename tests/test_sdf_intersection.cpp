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
#include <volplay/sdf_intersection.h>

namespace vp = volplay;

TEST_CASE("SDFIntersection")
{
    vp::SDFIntersection i(std::make_shared<vp::SDFSphere>(vp::S(2)), std::make_shared<vp::SDFSphere>(vp::S(0.5)));
    REQUIRE_CLOSE( i.eval(vp::Vector(0,0,0)), vp::S(-0.5) );
    REQUIRE_CLOSE( i.eval(vp::Vector(0.5,0,0)), vp::S(0) );
    REQUIRE_CLOSE( i.eval(vp::Vector(10,10,10)), vp::Vector(10,10,10).norm() - vp::S(0.5) );
    
}