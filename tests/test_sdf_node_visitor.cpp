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
#include <volplay/sdf_box.h>
#include <volplay/sdf_union.h>
#include <volplay/sdf_node_visitor.h>
#include <volplay/sdf_intersection.h>
#include <volplay/sdf_difference.h>
#include <volplay/sdf_repetition.h>
#include <volplay/sdf_rigid_transform.h>

namespace vp = volplay;

class CountVisitor : public vp::SDFNodeVisitor {
public:
	int countSphere = 0;
	int countPlane = 0;
	int countBox = 0;
	int countNode = 0;
	int countGroup = 0;
	int countUnion = 0;
	int countDifference = 0;
	int countIntersection = 0;
	int countRep = 0;
	int countTransform = 0;

	virtual void visit(vp::SDFNode *n) {
		++countNode;
		vp::SDFNodeVisitor::visit(n);
	}

	virtual void visit(vp::SDFSphere *n) {
		++countSphere;
		vp::SDFNodeVisitor::visit(n);
	}

	virtual void visit(vp::SDFPlane *n) {
		++countPlane;
		vp::SDFNodeVisitor::visit(n);
	}

	virtual void visit(vp::SDFBox *n){
		++countBox;
		vp::SDFNodeVisitor::visit(n);
	}

	virtual void visit(vp::SDFGroup *n){
		++countGroup;
		vp::SDFNodeVisitor::visit(n);
	}
	
	virtual void visit(vp::SDFUnion *n){
		++countUnion;
		vp::SDFNodeVisitor::visit(n);
	}

	virtual void visit(vp::SDFIntersection *n){
		++countIntersection;
		vp::SDFNodeVisitor::visit(n);
	}

	virtual void visit(vp::SDFDifference *n){
		++countDifference;
		vp::SDFNodeVisitor::visit(n);
	}

	virtual void visit(vp::SDFRigidTransform *n) {
		++countTransform;
		vp::SDFNodeVisitor::visit(n);
	}

	virtual void visit(vp::SDFRepetition *n) {
		++countRep;
		vp::SDFNodeVisitor::visit(n);
	}

};

TEST_CASE("SDFNodeVisitor")
{
	vp::SDFUnionPtr scene(new vp::SDFUnion());

	vp::SDFIntersectionPtr i(new vp::SDFIntersection());
	i->add(std::make_shared<vp::SDFPlane>());
	i->add(std::make_shared<vp::SDFSphere>());
	i->add(std::make_shared<vp::SDFBox>());
	scene->add(i);

	vp::SDFDifferencePtr d(new vp::SDFDifference());
	d->add(std::make_shared<vp::SDFPlane>());
	d->add(std::make_shared<vp::SDFSphere>());
	d->add(std::make_shared<vp::SDFBox>());
	i->add(d);

	vp::SDFUnionPtr u(new vp::SDFUnion());
	u->add(std::make_shared<vp::SDFPlane>());
	u->add(std::make_shared<vp::SDFSphere>());
	u->add(std::make_shared<vp::SDFBox>());
	i->add(u);

	vp::SDFRepetitionPtr r(new vp::SDFRepetition());
	r->add(std::make_shared<vp::SDFPlane>());
	r->add(std::make_shared<vp::SDFSphere>());
	r->add(std::make_shared<vp::SDFBox>());
	i->add(r);

	vp::SDFRigidTransformPtr t(new vp::SDFRigidTransform());
	t->add(std::make_shared<vp::SDFPlane>());
	t->add(std::make_shared<vp::SDFSphere>());
	t->add(std::make_shared<vp::SDFBox>());
	i->add(t);

	CountVisitor cv;

	scene->accept(cv);

	REQUIRE(cv.countNode == 21);
	REQUIRE(cv.countGroup == 6);	
	REQUIRE(cv.countBox == 5);
	REQUIRE(cv.countSphere == 5);
	REQUIRE(cv.countPlane == 5);
	REQUIRE(cv.countUnion == 2);
	REQUIRE(cv.countIntersection == 1);
	REQUIRE(cv.countDifference == 1);
	REQUIRE(cv.countRep == 1);
	REQUIRE(cv.countTransform == 1);

}

