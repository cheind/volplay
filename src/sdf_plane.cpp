// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_plane.h>
#include <volplay/sdf_node_visitor.h>

namespace volplay {
    
    SDFPlane::SDFPlane()
    : _normal(0,0,1), _w(0)
    {}
    
    SDFPlane::SDFPlane(const Vector &n)
    : _normal(n), _w(0)
    {}
    
    SDFResult
    SDFPlane::fullEval(const Vector &x) const
    {
        SDFResult r = {this, x.dot(_normal) + _w};
        return r;
    }

	void 
	SDFPlane::accept(SDFNodeVisitor &nv)
	{
		nv.visit(this);
	}
    
}