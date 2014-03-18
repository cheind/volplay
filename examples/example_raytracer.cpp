// This file is part of volplay, a library for interacting with volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include "catch.hpp"
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_repetition.h>
#include <volplay/sdf_making.h>
#include <volplay/ray_tracer.h>

namespace vp = volplay;

TEST_CASE("Raytracing simple scene using phong shading and a single light source")
{
    vp::SDFRepetitionPtr n = vp::makeSDFRepetition(vp::Vector(5, 5, 5), vp::makeSDFSphere(1));
}