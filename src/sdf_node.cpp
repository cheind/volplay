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
    
}