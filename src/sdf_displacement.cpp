// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_displacement.h>
#include <volplay/sdf_node_visitor.h>

namespace volplay {

    SDFDisplacement::SDFDisplacement()
    {}
        
    SDFDisplacement::SDFDisplacement(const DisplacementFnc &fnc)
        :_dfnc(fnc)
    {}
        
    SDFResult SDFDisplacement::fullEval(const Vector &x) const
    {
        SDFResult r = SDFUnion::fullEval(x);
        if (_dfnc) {
            Scalar d = _dfnc(x);
            r.sdf += d;
        }
        return r;
    }
        
    void SDFDisplacement::setDisplacementFunction(const DisplacementFnc &fnc)
    {
        _dfnc = fnc;
    }

	void SDFDisplacement::accept(SDFNodeVisitor &nv)
    {
        nv.visit(this);
        acceptChildren(nv);
    }
    
}