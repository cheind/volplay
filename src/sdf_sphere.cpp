// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_sphere.h>
#include <volplay/sdf_node_visitor.h>

namespace volplay {
    
    SDFSphere::SDFSphere()
    : _radius(1)
    {}
    
    SDFSphere::SDFSphere(Scalar radius)
    : _radius(radius)
    {}
    
    SDFResult
    SDFSphere::fullEval(const Vector &x) const
    {
        SDFResult r = {this, x.norm() - _radius};
        return r;
    }

	void SDFSphere::accept(SDFNodeVisitor &nv)
	{
		nv.visit(this);
	}
    
}