// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_make.h>
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_plane.h>
#include <volplay/sdf_box.h>
#include <volplay/sdf_union.h>
#include <volplay/sdf_intersection.h>
#include <volplay/sdf_difference.h>
#include <volplay/sdf_repetition.h>
#include <volplay/sdf_rigid_transform.h>

namespace volplay {
    
    SDFMake::SDFMake(SDFNodePtr n)
    :_n(n)
    {}
    
    SDFMake &
    SDFMake::join(const SDFMake &right)
    {
        _n = SDFUnionPtr(new SDFUnion(_n, right._n));
        return *this;
    }
    
    SDFMake &
    SDFMake::operator+(const SDFMake &right)
    {
        return join(right);
    }
    
    SDFMake &
    SDFMake::intersect(const SDFMake &right)
    {
        _n = SDFIntersectionPtr(new SDFIntersection(_n, right._n));
        return *this;
    }
    
    SDFMake &
    SDFMake::operator|(const SDFMake &right)
    {
        return intersect(right);
    }
    
    SDFMake &
    SDFMake::remove(const SDFMake &right)
    {
        _n = SDFDifferencePtr(new SDFDifference(_n, right._n));
        return *this;
    }
    
    SDFMake &
    SDFMake::operator-(const SDFMake &right)
    {
        return remove(right);
    }
    
    SDFMake &
    SDFMake::translate(const Vector &t)
    {
        AffineTransform at = AffineTransform::Identity();
        at.translate(t);
        _n = SDFRigidTransformPtr(new SDFRigidTransform(at, _n));
        return *this;
    }
    
    SDFMake &
    SDFMake::repeat(const Vector &cellSizes)
    {
        _n = SDFRepetitionPtr(new SDFRepetition(cellSizes, _n));
        return *this;
    }
    
    SDFMake &
    SDFMake::attach(const std::string &s, const SDFNodeAttachmentPtr &a)
    {
        _n->setAttachment(s, a);
        return *this;
    }
    
    SDFMake::operator SDFNodePtr() const
    {
        return _n;
    }
    
    // Static functions
    
    SDFMake SDFMake::wrap(const SDFNodePtr &n)
    {
        return SDFMake(n);
    }
    
    SDFMake SDFMake::sphere(Scalar radius)
    {
        return SDFMake(SDFNodePtr(new SDFSphere(radius)));
    }
    
    SDFMake SDFMake::plane()
    {
        return SDFMake(SDFPlanePtr(new SDFPlane()));
    }
    
    SDFMake SDFMake::plane(const Vector &normal)
    {
        return SDFMake(SDFPlanePtr(new SDFPlane(normal)));
    }

    SDFMake SDFMake::box()
    {
        return SDFMake(SDFBoxPtr(new SDFBox()));
    }
    
    SDFMake SDFMake::box(const Vector &halfExt)
    {
        return SDFMake(SDFBoxPtr(new SDFBox(halfExt)));
    }
    
    
}