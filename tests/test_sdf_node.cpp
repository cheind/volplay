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
#include <volplay/sdf_plane.h>
#include <volplay/sdf_make.h>
#include <volplay/sdf_node_attachment.h>

namespace vp = volplay;

class SDFTestAttachment : public vp::SDFNodeAttachment {
public:    
    int id;
};

TEST_CASE("SDFNode")
{
    vp::SDFSphere s;
    std::shared_ptr<SDFTestAttachment> a = s.getOrCreateAttachment<SDFTestAttachment>("id");
    a->id = 3;
    
    REQUIRE(s.attachment<SDFTestAttachment>("id")->id == 3);
    
    std::shared_ptr<SDFTestAttachment> b = s.getOrCreateAttachment<SDFTestAttachment>("id");
    REQUIRE(a.get() == b.get());
}

TEST_CASE("SDFNode::normal")
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
    
    // Test with planes
    vp::SDFNodePtr planes = vp::make()
        .join()
            .plane().normal(vp::Vector::UnitX())
            .plane().normal(vp::Vector::UnitY())
        .end();
    
    REQUIRE_CLOSE( (planes->normal(vp::Vector(0,0,0)) - vp::Vector(1,1,0).normalized()).norm(), 0);
}

TEST_CASE("SDFNode::trace")
{
    vp::SDFSphere s;
    vp::SDFNode::TraceOptions opts;
    vp::SDFNode::TraceResult tr;
    
    REQUIRE(opts.minT == 0);
    REQUIRE(opts.maxIter == 500);
    REQUIRE(opts.maxT == std::numeric_limits<vp::Scalar>::max());
    REQUIRE_CLOSE(opts.sdfThreshold, 0.001);
    REQUIRE(opts.stepFact == 1);
    
    REQUIRE(tr.t == 0);
    REQUIRE(tr.iter == 0);
    REQUIRE(tr.sdf == 0);
    
    REQUIRE_CLOSE( s.trace(vp::Vector(2, 0, 0), vp::Vector(-1, 0, 0), opts, &tr), 1);
    REQUIRE(tr.iter > 0); REQUIRE(tr.iter < 500); REQUIRE(tr.t == 1); REQUIRE_CLOSE(tr.sdf, 0);
    REQUIRE_CLOSE( s.trace(vp::Vector(-2, 0, 0), vp::Vector(1, 0, 0), opts, &tr), 1);
    REQUIRE(tr.iter > 0); REQUIRE(tr.iter < 500); REQUIRE(tr.t == 1); REQUIRE_CLOSE(tr.sdf, 0);
    // Inside, does nothing
    REQUIRE_CLOSE( s.trace(vp::Vector(0, 0, 0), vp::Vector(1, 0, 0), opts, &tr), 0);
    REQUIRE(tr.iter == 0); REQUIRE(tr.t == 0); REQUIRE(tr.sdf < 0);
}

