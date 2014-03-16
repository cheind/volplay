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
    };

}

#endif
