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

namespace vp = volplay;

TEST_CASE("Raytracer")
{
    vp::SDFSphere s;
    vp::SDFNode::SphereTraceOptions opts;
    
    REQUIRE(opts.minT == 0);
    REQUIRE(opts.maxIter == 500);
    REQUIRE(opts.maxT == std::numeric_limits<vp::Scalar>::max());
    REQUIRE_CLOSE(opts.sdfThreshold, 0.001);
    REQUIRE(opts.stepFact == 1);
    
    REQUIRE_CLOSE( s.sphereTrace(vp::Vector(2, 0, 0), vp::Vector(-1, 0, 0), opts), 1);
    REQUIRE_CLOSE( s.sphereTrace(vp::Vector(-2, 0, 0), vp::Vector(1, 0, 0), opts), 1);
    // Inside, does nothing
    REQUIRE_CLOSE( s.sphereTrace(vp::Vector(0, 0, 0), vp::Vector(1, 0, 0), opts), 0);
}
