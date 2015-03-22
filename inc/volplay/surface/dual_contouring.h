// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_DUAL_CONTOURING
#define VOLPLAY_DUAL_CONTOURING

#include <volplay/types.h>
#include <volplay/surface/types.h>
#include <volplay/fwd.h>

namespace volplay {

    namespace surface {

        /** 
            Surface extraction using Dual Contouring. 

            This implementation is based on uniform grids. Based on the work of

            Ju, Tao, et al. "Dual contouring of hermite data." 
            ACM Transactions on Graphics (TOG). Vol. 21. No. 3. ACM, 2002. 

            Schaefer, Scott, Joe Warren, and Rice UniversityE. 
            "Dual Contouring: $ The Secret SauceC." (2002).
        */
        class DualContouring {
        public:
            /** Empty initializer. */
            DualContouring();
        
            /** Extract surface. */
            IndexedSurface extractSurface(SDFNodePtr scene, const Vector &lower, const Vector &upper, const Vector &resolution);
        };

    }
}

#endif
