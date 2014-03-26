// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_RESULT
#define VOLPLAY_SDF_RESULT

#include <volplay/fwd.h>


namespace volplay {
    
    /** Represents the result querying the signed distance field at a specific location. */
    struct SDFResult {
        /** Closest node */
        const SDFNode *node;
        /** Signed distance */
        Scalar sdf;
    };
    
}

#endif