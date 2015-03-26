// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_intersection.h>
#include <volplay/util/iterator_range.h>
#include <volplay/sdf_node_visitor.h>

namespace volplay {
    
    SDFIntersection::SDFIntersection()
    {}
    
    SDFIntersection::SDFIntersection(const SDFNodePtr &left, const SDFNodePtr &right)
    {
        this->add(left);
        this->add(right);
    }
    
    SDFResult
    SDFIntersection::fullEval(const Vector &x) const
    {
        assert(this->size() > 0);
        
        SDFGroup::SDFNodeArray::const_iterator i = this->begin();
        
        SDFResult r = (*i)->fullEval(x);
        for (++i; i != this->end(); ++i) {
            SDFResult o = (*i)->fullEval(x);
            r = r.sdf > o.sdf ? r : o;
        }
        
        return r;
    }

	void SDFIntersection::accept(SDFNodeVisitor &nv)
	{
		nv.visit(this);
		for (auto iter = begin(); iter != end(); ++iter)
			(*iter)->accept(nv);
	}
    
}