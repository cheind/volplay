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
#include <volplay/sdf_rigid_transform.h>

namespace vp = volplay;

TEST_CASE("SDF derivative")
{
    vp::SDFSphere s;
    
    vp::Vector n = s.normal(vp::Vector(0, 0, 1), vp::Scalar(0.001));
    REQUIRE_CLOSE( n.norm(), 1);
    REQUIRE_CLOSE( n.dot(vp::Vector(0, 0, 1)), 1);
    
    n = s.normal(vp::Vector(0, 0, 0.5), vp::Scalar(0.001));
    REQUIRE_CLOSE( n.norm(), 1);
    REQUIRE_CLOSE( n.dot(vp::Vector(0, 0, 1)), 1);
    
    n = s.normal(vp::Vector(1, 0, 0), vp::Scalar(0.001));
    REQUIRE_CLOSE( n.norm(), 1);
    REQUIRE_CLOSE( n.dot(vp::Vector(1, 0, 0)), 1);
    
    n = s.normal(vp::Vector(1, 1, 0), vp::Scalar(0.001));
    REQUIRE_CLOSE( n.norm(), 1);
    REQUIRE_CLOSE( n.dot(vp::Vector(1, 1, 0).normalized()), 1);
    
    // Test if gradient always points into direction of maximum distance increase
    REQUIRE_CLOSE( (s.normal(vp::Vector(-1,0,0)) - vp::Vector(-1,0,0)).norm(), 0 );
    REQUIRE_CLOSE( (s.normal(vp::Vector(1,0,0)) - vp::Vector(1,0,0)).norm(), 0 );
    REQUIRE_CLOSE( (s.normal(vp::Vector(1,1,1)) - vp::Vector(1,1,1).normalized()).norm(), 0 );
    
    // Degenerate tests
    REQUIRE_CLOSE( (s.normal(vp::Vector(0,0,0)) - vp::Vector(0,0,0)).norm(), 0 );
}
