// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_node.h>
#include <limits>

namespace volplay {
    
    Scalar
    SDFNode::eval(const Vector &x) const
    {
        return this->fullEval(x).sdf;
    }
    
    Vector
    SDFNode::gradient(const Vector &x, Scalar eps) const
    {
        const Scalar invDenom = Scalar(1) / (Scalar(2) * eps);
        
        return Vector((eval(x + Vector(eps, 0, 0)) - eval(x + Vector(-eps, 0, 0))) * invDenom,
                      (eval(x + Vector(0, eps, 0)) - eval(x + Vector(0, -eps, 0))) * invDenom,
                      (eval(x + Vector(0, 0, eps)) - eval(x + Vector(0, 0, -eps))) * invDenom
        );
    }
    
    Vector
    SDFNode::normal(const Vector &x, Scalar eps) const
    {
        const Vector g = gradient(x, eps);
        const Scalar n = g.squaredNorm();
        if (n > 0) {
            return g / n;
        } else {
            return g;
        }
    }
    
    SDFNode::TraceOptions::TraceOptions()
    :minT(0), maxT(std::numeric_limits<Scalar>::max()), stepFact(1), sdfThreshold(0.001), maxIter(500)
    {
    }
    
    SDFNode::TraceResult::TraceResult()
    :iter(0), t(0), sdf(0)
    {
    }
    
    Scalar  
    SDFNode::trace(const Vector &o, const Vector &d, const TraceOptions &opts, TraceResult *tr) const
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
        
        Scalar t = opts.minT;
        Vector pos = o + t * d;
        Scalar sdf = eval(pos) * opts.stepFact;
        
        int nIter = 0;
        while (nIter < opts.maxIter && t < opts.maxT && sdf > opts.sdfThreshold) {
            t += sdf;
            pos += sdf * d;
            sdf = eval(pos) * opts.stepFact;
            ++nIter;
        }
        
        if (tr) {
            tr->t = t;
            tr->sdf = sdf;
            tr->iter = nIter;
        }
        
        return t;

    }
    
    
    
}