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
#include <volplay/sdf_union.h>
#include <volplay/sdf_making.h>

namespace vp = volplay;

TEST_CASE("SDFResult")
{
    vp::SDFSpherePtr s0 = vp::makeSDFSphere(1);
    vp::SDFSpherePtr s1 = vp::makeSDFSphere(1);
    
    vp::AffineTransform at = vp::AffineTransform::Identity();
    at.translate(vp::Vector(5, 0, 0));

    vp::SDFRigidTransformPtr r(new vp::SDFRigidTransform(at, s1));
    vp::SDFUnionPtr u(new vp::SDFUnion(s0, r));
    
    REQUIRE(u->fullEval(vp::Vector(0,0,0)).node == s0.get());
    REQUIRE(u->fullEval(vp::Vector(5,0,0)).node == s1.get());
    REQUIRE(u->fullEval(vp::Vector(0,10,0)).node == s0.get());
    REQUIRE(u->fullEval(vp::Vector(1,0,0)).node == s0.get());
    
}