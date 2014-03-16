// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_TRANSFORM
#define VOLPLAY_SDF_TRANSFORM

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <volplay/sdf_unary.h>

namespace volplay {

    /** Represents a rigid body transformation node. */
    class SDFRigidTransform : public SDFUnary {
    public:
        /** Empty transform initializer. Sets transform to identity. */
        SDFRigidTransform();
        
        /** Initialize from transform */
        SDFRigidTransform(const AffineTransform &localToWorld);
        
        /** Initialize from transform and node */
        SDFRigidTransform(const AffineTransform &localToWorld, const SDFNodePtr &n);
        
        /** Evaluate the SDF at given position. */
        virtual Scalar eval(const Vector &x) const;
        
        /** Access the stored transform */
        AffineTransform localToWorld() const;
        
        /** Access the stored transform */
        const AffineTransform &worldToLocal() const;
        
        /** Set a new transform */
        void setLocalToWorld(const AffineTransform &t);
        
    private:
        AffineTransform _worldToLocal;
    };

}

#endif
