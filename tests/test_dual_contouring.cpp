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

#include <volplay/volplay.h>

namespace vp = volplay;
namespace vps = volplay::surface;

TEST_CASE("DualContouring Sphere")
{
    vp::SDFNodePtr scene = vp::make().sphere().radius(1);

    vps::DualContouring dc;
    dc.setLowerBounds(vp::Vector(-2,-2,-2));
    dc.setUpperBounds(vp::Vector(2,2,2));
    dc.setResolution(vp::Vector::Constant(vp::S(0.1)));
    vps::IndexedSurface surface = dc.compute(scene);
    
    // Bounding box test

    Eigen::AlignedBox3f box;    
    for (vps::IndexedSurface::VertexMatrix::Index i = 0; i < surface.vertices.cols(); ++i) {
        box.extend(surface.vertices.col(i));
    }

    REQUIRE( box.min().isApprox(vp::Vector::Constant(vp::S(-1)), vp::S(0.01)) );
    REQUIRE( box.max().isApprox(vp::Vector::Constant(vp::S(1)), vp::S(0.01)) );

    // Normal test with triangles
    for (vps::IndexedSurface::FaceMatrix::Index i = 0; i < surface.faces.cols(); ++i) {        
        auto t = surface.faces.col(i);

        vp::Vector a = surface.vertices.col(t(1)) - surface.vertices.col(t(0));
        vp::Vector b = surface.vertices.col(t(2)) - surface.vertices.col(t(0));
        vp::Vector n = a.cross(b).normalized();

        if (n.allFinite()) {  // TODO, seems like we also have some degenerate faces. 
                              // Maybe when intersection is on corner, then all 4 vertices are on that corner.      
            vp::Vector c = surface.vertices.col(t(1)) + surface.vertices.col(t(2)) + surface.vertices.col(t(0));
            c /= vp::S(3);

            vp::S cosangle = n.dot(c.normalized());
            REQUIRE_CLOSE_PREC(cosangle, vp::S(1), 0.01);
        }
    }
}

TEST_CASE("DualContouring Sphere with Offset")
{
    vp::SDFNodePtr scene = vp::make().sphere().radius(1);

    vps::DualContouring dc;
    dc.setLowerBounds(vp::Vector(-2,-2,-2));
    dc.setUpperBounds(vp::Vector(2,2,2));
    dc.setResolution(vp::Vector::Constant(vp::S(0.1)));
    dc.setIsoLevel(vp::S(0.1));
    vps::IndexedSurface surface = dc.compute(scene);
    
    // Bounding box test

    Eigen::AlignedBox3f box;    
    for (vps::IndexedSurface::VertexMatrix::Index i = 0; i < surface.vertices.cols(); ++i) {
        box.extend(surface.vertices.col(i));
    }

    REQUIRE( box.min().isApprox(vp::Vector::Constant(vp::S(-1.1)), vp::S(0.01)) );
    REQUIRE( box.max().isApprox(vp::Vector::Constant(vp::S(1.1)), vp::S(0.01)) );

    // Normal test with triangles
    for (vps::IndexedSurface::FaceMatrix::Index i = 0; i < surface.faces.cols(); ++i) {        
        auto t = surface.faces.col(i);

        vp::Vector a = surface.vertices.col(t(1)) - surface.vertices.col(t(0));
        vp::Vector b = surface.vertices.col(t(2)) - surface.vertices.col(t(0));
        vp::Vector n = a.cross(b).normalized();

        if (n.allFinite()) {  // TODO, seems like we also have some degenerate faces. 
                              // Maybe when intersection is on corner, then all 4 vertices are on that corner.      
            vp::Vector c = surface.vertices.col(t(1)) + surface.vertices.col(t(2)) + surface.vertices.col(t(0));
            c /= vp::S(3);

            vp::S cosangle = n.dot(c.normalized());
            REQUIRE_CLOSE_PREC(cosangle, vp::S(1), 0.01);
        }
    }
}