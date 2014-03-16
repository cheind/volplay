// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_NODE
#define VOLPLAY_SDF_NODE

#include <volplay/types.h>

namespace volplay {

    /** A leaf node in the SDF (signed distance function) scene graph. */
    class SDFNode {
    public:
        /** Evaluate the SDF at given position. */
        virtual Scalar eval(const Vector &x) const = 0;
        
        /** Evaluate the gradient of the SDF at the given position.
         *  The gradient will always point in the direction of maximum distance increase.
         */
        virtual Vector gradient(const Vector &x, Scalar eps = Scalar(0.0001)) const;
        
        /** Calculate the approximate unit normal at the given position. */
        virtual Vector normal(const Vector &x, Scalar eps = Scalar(0.0001)) const;
    };

}

#endif
