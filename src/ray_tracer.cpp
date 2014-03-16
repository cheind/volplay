// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/ray_tracer.h>
#include <volplay/sdf_node.h>
#include <limits>

namespace volplay {
    
    const RayTracer::TraceConstraints &
    RayTracer::TraceConstraints::defaults()
    {
        static TraceConstraints tc = {0, std::numeric_limits<Scalar>::max(), 500};
        return tc;
    }
    
    RayTracer::RayTracer()
    :_iso(0), _isoTh(0.01)
    {}
    
    RayTracer::RayTracer(const SDFNodeConstPtr &n)
    : _n(n), _iso(0), _isoTh(0.01)
    {}
    
    const SDFNodeConstPtr &
    RayTracer::rootNode()
    {
        return _n;
    }
    
    void
    RayTracer::setRootNode(const SDFNodeConstPtr &n)
    {
        _n = n;
    }
    
    void
    RayTracer::setIsoThreshold(Scalar th)
    {
        _isoTh = th;
    }
    
    void
    RayTracer::setIsoValue(Scalar iso)
    {
        _iso = iso;
    }
    
    Scalar
    RayTracer::trace(const Vector &o, const Vector &d, const TraceConstraints &c) const
    {
        // Performs sphere tracing. That is given a position along the ray, the SDF at
        // this position tells us how far the next surface intersection is at least away.
        // That is we can move the position along the ray forward by the value of the SDF.
        //
        // The method performs generally well but requires many evaluations of the SDF
        // around edges of objects.
        //
        // Note that the underlying assumption made by this algorithm is that nodes might
        // underestimate the true distance, but do not overestimate it.
        
        Scalar t = c.minT;
        Vector pos = o + t * d;
        Scalar sdf = _n->eval(pos);
        
        int nIter = 0;
        while (nIter < c.maxIter && t < c.maxT && fabs(sdf - _iso) > _isoTh) {
            t += sdf;
            pos += sdf * d;
            sdf = _n->eval(pos);
            ++nIter;
        }
        
        return t;
    }
    
}