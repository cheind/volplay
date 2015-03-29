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
#include <volplay/sdf_displacement.h>
#include <deque>

#ifdef _MSC_VER
#pragma warning (disable: 4355)
#endif 

namespace volplay {
    namespace detail {

        // Root

        MakeRoot::MakeRoot()
            : MakeBaseType(this)
        {}

        SDFNodePtr MakeRoot::node() const
        {
            return _nodes.back();
        }

        void MakeRoot::addNode(SDFNodePtr n)
        {
            if (!n) // Necessary for root itself
                return;

            if (_nodes.empty()) {
                // Top node is always pushed					
                _nodes.push_front(n);
            }
            else if (SDFGroupPtr g = asGroup(_nodes.front())) {
                g->add(n);
                if (n->isGroup())
                    _nodes.push_front(n);
            }
        }

        void MakeRoot::bubbleUp() {
            if (_nodes.size() <= 1) {
                return;
            }

            do {
                _nodes.erase(_nodes.begin());
            } while (_nodes.size() > 1 && !(asGroup(_nodes.front())));
        }

        SDFNodePtr MakeRoot::createNode() const
        {
            return SDFNodePtr();
        }

        SDFGroupPtr MakeRoot::asGroup(SDFNodePtr n)
        {
            return std::dynamic_pointer_cast<SDFGroup>(n);
        }

        // Union

        MakeJoin::MakeJoin(MakeRoot *r)
            : MakeBaseType(r)
        {}

        SDFNodePtr MakeJoin::createNode() const
        {
            return std::make_shared<SDFUnion>();
        }


        // Intersection
            
        MakeIntersection::MakeIntersection(MakeRoot *r)
            : MakeBaseType(r)
        {}

        SDFNodePtr MakeIntersection::createNode() const
        {
            return std::make_shared<SDFIntersection>();
        }

        // Difference
            
        MakeDifference::MakeDifference(MakeRoot *r)
            : MakeBaseType(r)
        {}

        SDFNodePtr MakeDifference::createNode() const
        {
            return std::make_shared<SDFDifference>();
        }

        // Sphere

        MakeSphere::MakeSphere(MakeRoot *r)
            : MakeBaseType(r), _radius(1)
        {}

        MakeSphere &MakeSphere::radius(float r)
        {
            _radius = r;
            return *this;
        }

        SDFNodePtr MakeSphere::createNode() const
        {
            return std::make_shared<SDFSphere>(_radius);
        }

        // Plane

        MakePlane::MakePlane(MakeRoot *r)
            : MakeBaseType(r), _n(Vector(0,0,1))
        {}

        MakePlane &MakePlane::normal(const Vector &n)
        {
            _n = n.normalized();
            return *this;
        }

        SDFNodePtr MakePlane::createNode() const
        {
            return std::make_shared<SDFPlane>(_n);
        }

        // Box

        MakeBox::MakeBox(MakeRoot *r)
                : MakeBaseType(r), _hl(Vector(S(0.5), S(0.5), S(0.5)))
        {}

        MakeBox &MakeBox::halfLengths(const Vector &hl)
        {
            _hl = hl;
            return *this;
        }

        MakeBox &MakeBox::lengths(const Vector &hl)
        {
            _hl = hl * S(0.5);
            return *this;
        }

        SDFNodePtr MakeBox::createNode() const
        {
            return std::make_shared<SDFBox>(_hl);
        }

        // Transform

        MakeTransform::MakeTransform(MakeRoot *r)
            : MakeBaseType(r)
        {
            _t.setIdentity();
        }

        MakeTransform &MakeTransform::transform(const AffineTransform &t) {
            _t = t;
            return *this;
        }

        MakeTransform &MakeTransform::matrix(const AffineTransform::MatrixType &m) {
            _t.matrix() = m;
            return *this;
        }

        SDFNodePtr MakeTransform::createNode() const
        {
            return std::make_shared<SDFRigidTransform>(_t);
        }

        // Repetition

        MakeRepetition::MakeRepetition(MakeRoot *r)
            : MakeBaseType(r), _cellSizes(Vector::Constant(std::numeric_limits<Scalar>::infinity()))
        {}

        MakeRepetition &MakeRepetition::cellSizes(const Vector &s)
        {
            _cellSizes = s;
            return *this;
        }

        MakeRepetition &MakeRepetition::x(Scalar s)
        {
            _cellSizes.x() = s;
            return *this;
        }

        MakeRepetition &MakeRepetition::y(Scalar s)
        {
            _cellSizes.y() = s;
            return *this;
        }

        MakeRepetition &MakeRepetition::z(Scalar s)
        {
            _cellSizes.z() = s;
            return *this;
        }

        SDFNodePtr MakeRepetition::createNode() const
        {
            return std::make_shared<SDFRepetition>(_cellSizes);
        }

        // SDF Displacement

       MakeDisplacement::MakeDisplacement(MakeRoot *r)
           :MakeBaseType(r)
       {}

       MakeDisplacement &MakeDisplacement::fnc(const ScalarFnc &f)
       {
           _fnc = f;
           return *this;
       }

       SDFNodePtr MakeDisplacement::createNode() const
       {
           return std::make_shared<SDFDisplacement>(_fnc);
       }
    }

    detail::MakeRoot make()
    {
        return detail::MakeRoot();
    }
    
}