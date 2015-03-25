// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_rigid_transform.h>

namespace volplay {
    
    SDFRigidTransform::SDFRigidTransform()
    : _worldToLocal(AffineTransform::Identity())
    {}
    
    SDFRigidTransform::SDFRigidTransform(const AffineTransform &t)
    : _worldToLocal(t.inverse())
    {}
    
    SDFRigidTransform::SDFRigidTransform(const AffineTransform &t, const SDFNodePtr &n)
    : _worldToLocal(t.inverse())
    {
		this->add(n);
	}
    
    const AffineTransform &
    SDFRigidTransform::worldToLocal() const
    {
        return _worldToLocal;
    }
    
    AffineTransform
    SDFRigidTransform::localToWorld() const
    {
        return _worldToLocal.inverse();
    }
    
    void
    SDFRigidTransform::setLocalToWorld(const AffineTransform &t)
    {
        _worldToLocal = t.inverse();
    }

    
    SDFResult
    SDFRigidTransform::fullEval(const Vector &x) const
    {
        return SDFUnion::fullEval(_worldToLocal * x);
    }
    
    
}