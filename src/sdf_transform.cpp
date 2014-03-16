// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_transform.h>

namespace volplay {
    
    SDFTransform::SDFTransform()
    : _worldToLocal(AffineTransform::Identity())
    {}
    
    SDFTransform::SDFTransform(const AffineTransform &t)
    : _worldToLocal(t.inverse())
    {}
    
    SDFTransform::SDFTransform(const AffineTransform &t, const SDFNodePtr &n)
    : _worldToLocal(t.inverse()), _n(n)
    {}
    
    const AffineTransform &
    SDFTransform::worldToLocal() const
    {
        return _worldToLocal;
    }
    
    AffineTransform
    SDFTransform::localToWorld() const
    {
        return _worldToLocal.inverse();
    }
    
    void
    SDFTransform::setLocalToWorld(const AffineTransform &t)
    {
        _worldToLocal = t.inverse();
    }
    
    const SDFNodePtr &
    SDFTransform::node() const
    {
        return _n;
    }
    
    void
    SDFTransform::setNode(const SDFNodePtr &n)
    {
        _n = n;
    }

    
    Scalar
    SDFTransform::eval(const Vector &x) const
    {
        assert(_n);
        
        return _n->eval(_worldToLocal * x);
    }
    
    
}