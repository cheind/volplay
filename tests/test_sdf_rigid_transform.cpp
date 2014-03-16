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

TEST_CASE("SDFRigidTransform")
{
    vp::SDFRigidTransform t(vp::AffineTransform::Identity(), std::make_shared<vp::SDFSphere>());
    REQUIRE_CLOSE( t.eval(vp::Vector(0,0,0)), vp::S(-1) );
    REQUIRE_CLOSE( t.eval(vp::Vector(1,0,0)), vp::S(0) );
    REQUIRE_CLOSE( t.eval(vp::Vector(10,10,10)), vp::Vector(10,10,10).norm() - vp::S(1) );
    
    vp::AffineTransform at = vp::AffineTransform::Identity();
    at.translate(vp::Vector(5, 0, 0));
    vp::SDFRigidTransform t2(at, std::make_shared<vp::SDFSphere>());
    REQUIRE_CLOSE( t2.eval(vp::Vector(0,0,0)), vp::S(4) );
    REQUIRE_CLOSE( t2.eval(vp::Vector(5,0,0)), vp::S(-1) );
}