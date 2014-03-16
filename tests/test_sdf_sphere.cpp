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

TEST_CASE("SDFSphere")
{
    vp::SDFSphere s;
    REQUIRE_CLOSE( s.eval(vp::Vector(0,0,0)), vp::S(-1) );
    REQUIRE_CLOSE( s.eval(vp::Vector(1,0,0)), vp::S(0) );
    REQUIRE_CLOSE( s.eval(vp::Vector(10,10,10)), vp::Vector(10,10,10).norm() - vp::S(1) );
    
    vp::SDFSphere s2(vp::S(2));
    for (vp::S alpha = 0; alpha < vp::S(2 * 3.14); alpha += vp::S(0.2)) {
        for (vp::S beta = 0; beta < vp::S(3.14); beta += vp::S(0.2)) {
            
            REQUIRE_CLOSE( s.eval(vp::Vector(cosf(alpha)*sinf(beta),sinf(alpha)*sinf(beta),cosf(beta))), vp::S(0) );
            REQUIRE_CLOSE( s2.eval(vp::Vector(vp::S(2)*cosf(alpha)*sinf(beta),vp::S(2)*sinf(alpha)*sinf(beta),vp::S(2)*cosf(beta))), vp::S(0) );
        }
    }
    
}