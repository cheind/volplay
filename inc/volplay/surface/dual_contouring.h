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

            /** Set the lower bound of the volume to be reconstructed. */
            void setLowerBounds(const Vector &lower);

            /** Set the lower bound of the volume to be reconstructed. */
            void setUpperBounds(const Vector &upper);

            /** Set the resolution of the uniform grid. */
            void setResolution(const Vector &resolution);

            /** Set the iso value at which to contour. Defaults to zero. */
            void setIsoLevel(Scalar iso);

            /** Determine how the algorithm computes edge intersections and vertex locations. */
            enum EComputeType {
                /** Use non linear edge intersection model and place vertices by solving the QEF of Dual Contouring. */
                COMPUTE_NONLINEAR_DC,
                /** Use linear edge intersection model and place vertices by solving the QEF of Dual Contouring. */
                COMPUTE_LINEAR_DC,
                /** Place Vertices at midpoints of cells. This results in a Minecraft style world. */
                COMPUTE_MIDPOINT
            };

            /** Extract the surface. */
            IndexedSurface compute(SDFNodePtr scene, EComputeType et = COMPUTE_NONLINEAR_DC);
        
        private:
            Vector _lower, _upper, _resolution;
            Scalar _iso;
        };

    }
}

#endif
