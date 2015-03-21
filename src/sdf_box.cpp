// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2015 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_box.h>

namespace volplay {
    
    SDFBox::SDFBox()
        : _hext(Vector::Constant(S(0.5)))
    {}
    
    SDFBox::SDFBox(const Vector &halfExt)
    : _hext(halfExt)
    {}
    
    SDFResult
    SDFBox::fullEval(const Vector &x) const
    {        
        Vector d = x.array().abs().matrix() - _hext;
        S sdf = std::min<S>(d.maxCoeff(), S(0)) + d.array().max(S(0)).matrix().norm();
        SDFResult r = {this, sdf};
        return r;
    }
    
}