// This file is part of volplay, a library for interacting with volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include "catch.hpp"

#include <volplay/sdf_sphere.h>
#include <volplay/sdf_plane.h>
#include <volplay/sdf_make.h>
#include <volplay/surface/dual_contouring.h>
#include <volplay/surface/off_export.h>

namespace vp = volplay;
namespace vps = volplay::surface;


TEST_CASE("surface_export")
{
   
    vp::SDFNodePtr scene = vp::SDFMake::sphere(1).translate(vp::Vector(0.01f, 0.01f, 0.01f));
    vps::DualContouring dc;
    vps::IndexedSurface surface = dc.extractSurface(scene, vp::Vector(-2,-2,-2), vp::Vector(2,2,2), vp::Vector::Constant(0.1f));
    vps::OFFExport off;
    off.exportSurface("test.off", surface);
    

}