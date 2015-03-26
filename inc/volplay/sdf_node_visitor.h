// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_NODE_VISITOR
#define VOLPLAY_SDF_NODE_VISITOR

#include <volplay/types.h>
#include <volplay/fwd.h>

namespace volplay {

    /** Base visitor for SDF node hierarchies. 
	
		By default each visit method recursively calls visit
		methods for parents in the class hierarchy. Therefore, when overloading
		a method in a subclass invoke the base SDFNodeVisitor::visit as well.
		
	*/
    class SDFNodeVisitor {
    public:

		/* Visit node */
		virtual void visit(SDFNode *n);

		/* Visit node */
		virtual void visit(SDFSphere *n);

		/* Visit node */
		virtual void visit(SDFPlane *n);

		/* Visit node */
		virtual void visit(SDFBox *n);

		/* Visit node */
		virtual void visit(SDFGroup *n);

		/* Visit node */
		virtual void visit(SDFUnion *n);

		/* Visit node */
		virtual void visit(SDFIntersection *n);

		/* Visit node */
		virtual void visit(SDFDifference *n);

		/* Visit node */
		virtual void visit(SDFRigidTransform *n);

		/* Visit node */
		virtual void visit(SDFRepetition *n);	
    };

}

#endif
