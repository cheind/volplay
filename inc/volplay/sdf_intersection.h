// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_INTERSECTION
#define VOLPLAY_SDF_INTERSECTION

#include <volplay/types.h>
#include <volplay/sdf_group.h>

namespace volplay {

    /** Represents the n-ary intersection of nodes. */
    class SDFIntersection : public SDFGroup {
    public:
        /** Empty intersection initializer. */
        SDFIntersection();
        
        /** Binary intersection initializer. */
        SDFIntersection(const SDFNodePtr &left, const SDFNodePtr &right);
        
        /** Evaluate the SDF at given position. */
        virtual SDFResult fullEval(const Vector &x) const;
    };

}

#endif
