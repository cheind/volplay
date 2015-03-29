// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_node_visitor.h>
#include <volplay/sdf_node.h>
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_box.h>
#include <volplay/sdf_plane.h>
#include <volplay/sdf_group.h>
#include <volplay/sdf_union.h>
#include <volplay/sdf_intersection.h>
#include <volplay/sdf_difference.h>
#include <volplay/sdf_rigid_transform.h>
#include <volplay/sdf_repetition.h>
#include <volplay/sdf_displacement.h>

namespace volplay {
    
	void SDFNodeVisitor::visit(SDFNode *n)
	{
		// Nothing todo
	}

	void SDFNodeVisitor::visit(SDFSphere *n)
	{
		visit(static_cast<SDFNode*>(n));
	}

	void SDFNodeVisitor::visit(SDFPlane *n)
	{
		visit(static_cast<SDFNode*>(n));
	}

	void SDFNodeVisitor::visit(SDFBox *n)
	{
		visit(static_cast<SDFNode*>(n));
	}

	void SDFNodeVisitor::visit(SDFGroup *n)
	{
		visit(static_cast<SDFNode*>(n));
	}

	void SDFNodeVisitor::visit(SDFUnion *n)
	{
		visit(static_cast<SDFGroup*>(n));
	}

	void SDFNodeVisitor::visit(SDFIntersection *n)
	{
		visit(static_cast<SDFGroup*>(n));
	}

	void SDFNodeVisitor::visit(SDFDifference *n)
	{
		visit(static_cast<SDFGroup*>(n));
	}

	void SDFNodeVisitor::visit(SDFRigidTransform *n)
	{
		visit(static_cast<SDFUnion*>(n));
	}

	void SDFNodeVisitor::visit(SDFRepetition *n)
	{
		visit(static_cast<SDFUnion*>(n));
	}

    void SDFNodeVisitor::visit(SDFDisplacement *n)
	{
		visit(static_cast<SDFUnion*>(n));
	}
	    
}