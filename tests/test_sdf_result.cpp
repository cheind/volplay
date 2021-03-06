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
#include <volplay/sdf_make.h>

namespace vp = volplay;

TEST_CASE("SDFResult")
{
    vp::SDFNodePtr s0;
    vp::SDFNodePtr s1;

    vp::SDFNodePtr u = vp::make()
        .join()
            .sphere().storeNodePtr(&s0)
            .transform().translate(vp::Vector(5, 0, 0))
                .sphere().storeNodePtr(&s1)
            .end()
        .end();
    
    REQUIRE(u->fullEval(vp::Vector(0,0,0)).node == s0.get());
    REQUIRE(u->fullEval(vp::Vector(5,0,0)).node == s1.get());
    REQUIRE(u->fullEval(vp::Vector(0,10,0)).node == s0.get());
    REQUIRE(u->fullEval(vp::Vector(1,0,0)).node == s0.get());
    
}