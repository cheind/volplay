// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_SPHERE
#define VOLPLAY_SDF_SPHERE

#include <volplay/types.h>
#include <volplay/sdf_node.h>

namespace volplay {

    /** A sphere primitive centered around the origin. */
    class SDFSphere : public SDFNode {
    public:
        /** Create a new unit sphere. */
        SDFSphere();
        
        /** Create a new sphere with custom radius */
        SDFSphere(Scalar radius);
        
        /** Evaluate the SDF at given position. */
        virtual SDFResult fullEval(const Vector &x) const;
    private:
        Scalar _radius;
    };

}

#endif
