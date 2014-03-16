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
#include <volplay/ray_tracer.h>

namespace vp = volplay;

TEST_CASE("Raytracer")
{
    vp::RayTracer rt(std::make_shared<vp::SDFSphere>());
    vp::RayTracer::TraceConstraints tc = vp::RayTracer::TraceConstraints::defaults();
    
    REQUIRE_CLOSE( rt.trace(vp::Vector(2, 0, 0), vp::Vector(-1, 0, 0), tc), 1);
    REQUIRE_CLOSE( rt.trace(vp::Vector(-2, 0, 0), vp::Vector(1, 0, 0), tc), 1);
    // Inside, travels into opposite direction.
    REQUIRE_CLOSE( rt.trace(vp::Vector(0, 0, 0), vp::Vector(1, 0, 0), tc), -1);
}
