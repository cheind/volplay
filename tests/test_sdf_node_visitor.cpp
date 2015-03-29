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

class CountVisitor : public vp::SDFNodeVisitor {
public:
	int countSphere;
	int countPlane;
	int countBox;
	int countNode;
	int countGroup;
	int countUnion;
	int countDifference;
	int countIntersection;
	int countRep;
	int countTransform;
    int countDisplacement;

    CountVisitor()
    {
        countSphere = 0;
        countPlane = 0;
        countBox = 0;
        countNode = 0;
        countGroup = 0;
        countUnion = 0;
        countDifference = 0;
        countIntersection = 0;
        countRep = 0;
        countTransform = 0;
        countDisplacement = 0;
    }

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

    virtual void visit(vp::SDFDisplacement *n) {
		++countDisplacement;
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

    vp::SDFDisplacementPtr disp(new vp::SDFDisplacement());
	scene->add(disp);

	CountVisitor cv;

	scene->accept(cv);

	REQUIRE(cv.countNode == 22);
	REQUIRE(cv.countGroup == 7);	
	REQUIRE(cv.countBox == 5);
	REQUIRE(cv.countSphere == 5);
	REQUIRE(cv.countPlane == 5);
	REQUIRE(cv.countUnion == 5);
	REQUIRE(cv.countIntersection == 1);
	REQUIRE(cv.countDifference == 1);
	REQUIRE(cv.countRep == 1);
	REQUIRE(cv.countTransform == 1);
    REQUIRE(cv.countDisplacement == 1);

}

