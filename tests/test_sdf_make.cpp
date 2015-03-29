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
#include <volplay/sdf_box.h>
#include <volplay/sdf_rigid_transform.h>
#include <volplay/sdf_union.h>
#include <volplay/sdf_make.h>
#include <volplay/sdf_node_visitor.h>

namespace vp = volplay;

class DFSVisitor : public vp::SDFNodeVisitor {
public:
    enum NodeType {
        Unknown,
        Union,
        Transform,
        Repetition,
        Displacement,
        Sphere,
        Box
    };

    std::vector<NodeType> visited;

    virtual void visit(vp::SDFNode *n) {
        visited.push_back(Unknown);
	}

    virtual void visit(vp::SDFSphere *n) {
        visited.push_back(Sphere);
	}

    virtual void visit(vp::SDFUnion *n) {
        visited.push_back(Union);
	}

    virtual void visit(vp::SDFBox *n) {
        visited.push_back(Box);
	}

    virtual void visit(vp::SDFRigidTransform *n) {
        visited.push_back(Transform);
	}

    virtual void visit(vp::SDFRepetition *n) {
        visited.push_back(Repetition);
	}

    virtual void visit(vp::SDFDisplacement *n) {
        visited.push_back(Displacement);
	}
};

TEST_CASE("SDFMake - Hierarchy")
{
    vp::SDFNodePtr u = vp::make()
        .join()
            .transform().translate(vp::Vector(5, 0, 0))
                .repetition()
                    .box().lengths(vp::Vector(1))
                    .box()
                .end()
                .displacement()
                    .sphere()
                .end()
            .end()
            .sphere()
        .end();

    DFSVisitor v;
    u->accept(v);

    std::vector<DFSVisitor::NodeType> shouldBe;
    shouldBe.push_back(DFSVisitor::Union);
    shouldBe.push_back(DFSVisitor::Transform);
    shouldBe.push_back(DFSVisitor::Repetition);
    shouldBe.push_back(DFSVisitor::Box);    
    shouldBe.push_back(DFSVisitor::Box);    
    shouldBe.push_back(DFSVisitor::Displacement);
    shouldBe.push_back(DFSVisitor::Sphere);
    shouldBe.push_back(DFSVisitor::Sphere);
    
    REQUIRE( shouldBe == v.visited );

}


TEST_CASE("SDFMake - StorePtr")
{
    vp::SDFNodePtr s0;
    vp::SDFNodePtr s1;

    vp::SDFNodePtr u = vp::make()
        .join()
            .sphere().storeNodePtr(&s0)
            .transform().translate(vp::Vector(5, 0, 0))
                .box().lengths(vp::Vector(1)).storeNodePtr(&s1)
            .end()
        .end();
    
    REQUIRE( std::dynamic_pointer_cast<vp::SDFSphere>(s0) );
    REQUIRE( std::dynamic_pointer_cast<vp::SDFBox>(s1) );
}