// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_RAY_TRACER
#define VOLPLAY_RAY_TRACER

#include <volplay/types.h>
#include <volplay/fwd.h>

namespace volplay {

    /** Sphere tracing of rays through SDF fields. Purely CPU based. */
    class RayTracer {
    public:
        
        /** Constraints while tracing a ray. */
        struct TraceConstraints {
            Scalar minT;
            Scalar maxT;
            Scalar maxIter;
            
            /** Access trace defaults */
            static const TraceConstraints &defaults();
        };
        
        /** Empty ray tracer */
        RayTracer();
        
        /** Initialize with root node to trace. */
        RayTracer(const SDFNodeConstPtr &n);
        
        /** Access the root node. */
        const SDFNodeConstPtr &rootNode();
        
        /** Set root node. */
        void setRootNode(const SDFNodeConstPtr &n);
        
        /** Set the threshold below which the absolute SDF value is considered within target iso value. */
        void setIsoThreshold(Scalar th);
        
        /** Set the iso value to be considered a hit while tracing rays. */
        void setIsoValue(Scalar iso);
        
        /** Trace a ray searching for the closest intersection between the ray and the SDF. */
        Scalar trace(const Vector &o, const Vector &d, const TraceConstraints &c) const;

    private:
        SDFNodeConstPtr _n;
        Scalar _iso, _isoTh;
    };

}

#endif
