// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_making.h>
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_plane.h>
#include <volplay/sdf_union.h>
#include <volplay/sdf_intersection.h>
#include <volplay/sdf_difference.h>
#include <volplay/sdf_repetition.h>
#include <volplay/sdf_rigid_transform.h>

namespace volplay {
    namespace making {
    
        MakeWrapper::MakeWrapper(SDFNodePtr n)
        :_n(n)
        {}
        
        MakeWrapper &
        MakeWrapper::join(const MakeWrapper &right)
        {
            _n = SDFUnionPtr(new SDFUnion(_n, right._n));
            return *this;
        }
        
        MakeWrapper &
        MakeWrapper::intersect(const MakeWrapper &right)
        {
            _n = SDFIntersectionPtr(new SDFIntersection(_n, right._n));
            return *this;
        }
        
        MakeWrapper &
        MakeWrapper::remove(const MakeWrapper &right)
        {
            _n = SDFDifferencePtr(new SDFDifference(_n, right._n));
            return *this;
        }
        
        MakeWrapper &
        MakeWrapper::translate(const Vector &t)
        {
            AffineTransform at = AffineTransform::Identity();
            at.translate(t);
            _n = SDFRigidTransformPtr(new SDFRigidTransform(at, _n));
            return *this;
        }
        
        MakeWrapper &
        MakeWrapper::repeat(const Vector &cellSizes)
        {
            _n = SDFRepetitionPtr(new SDFRepetition(cellSizes, _n));
            return *this;
        }
        
        MakeWrapper::operator SDFNodePtr() const
        {
            return _n;
        }
        
        // Free functions
        
        MakeWrapper wrap(const SDFNodePtr &n)
        {
            return MakeWrapper(n);
        }
        
        MakeWrapper sphere(Scalar radius)
        {
            return MakeWrapper(SDFNodePtr(new SDFSphere(radius)));
        }
        
        MakeWrapper plane()
        {
            return MakeWrapper(SDFPlanePtr(new SDFPlane()));
        }
        
    }    
    
}