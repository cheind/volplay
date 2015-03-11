// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SURFACE_TYPES
#define VOLPLAY_SURFACE_TYPES

#include <volplay/types.h>
#include <vector>

namespace volplay {

    namespace surface {

        /** Surface defined by vertices and faces that index vertices. */
        struct IndexedSurface {
            Eigen::Matrix<Scalar, 3, Eigen::Dynamic> vertices;
            Eigen::MatrixXi faces;            
        };

    }    
}

#endif
