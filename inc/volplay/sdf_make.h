// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_MAKE
#define VOLPLAY_SDF_MAKE

#include <volplay/sdf_node.h>
#include <volplay/types.h>
#include <volplay/fwd.h>
#include <deque>

namespace volplay {
    namespace make {
        namespace detail {

            class MakeRoot;
            class MakeSphere;
            class MakeBox;
            class MakePlane;
            class MakeJoin;
            class MakeIntersection;
            class MakeDifference;
            class MakeTransform;
            class MakeRepetition;
            

            /** Base class for making. */
            template<class Derived>
            class MakeBase {
            public:
                typedef MakeBase<Derived> MakeBaseType;

                /** Create a new SDFSphere */
                MakeSphere sphere();

                /** Create a new SDFBox */
                MakeBox box();

                /** Create a new SDFBox */
                MakePlane plane();

                /** Create a new SDFUnion */
                MakeJoin join();

                /** Create a new SDFIntersection */
                MakeIntersection intersection();

                /** Create a new SDFIntersection */
                MakeDifference difference();

                /** Create a new SDFRigidTransform */
                MakeTransform transform();

                /** Create a new SDFRepetition */
                MakeRepetition repetition();

                /** Add a new attachment */
                Derived &attach(const std::string &key, SDFNodeAttachmentPtr attachment);

                /** Helper method to assign node pointer once it is created */
                Derived &storeNodePtr(SDFNodePtr *ptr);

                /** Complete the current SDFGroup and bubble up to parent. */
                Derived &end();

                /** Convert to SDFNodePtr */
                operator SDFNodePtr();
            protected:

                MakeBase(MakeRoot *r);
                void deferredAttachNode();

                MakeRoot *_root;
                bool _isAttached;
                SDFNodePtr *_storePtr;
                SDFNode::AttachmentMap _attachments;
            };

            /** Helper object that maintains the hierarchy of creation. */
            class MakeRoot : public MakeBase< MakeRoot >
            {
            public:
                /** Initialize empty hierarchy */
                MakeRoot();

                /** Access root node of hierarhcy */
                SDFNodePtr node() const;

                /** Add child node to current hiearchy elementy */
                void addNode(SDFNodePtr n);

                /** Bubble up to parent. */
                void bubbleUp();

                /** Returns null pointer */
                SDFNodePtr createNode() const;

            private:
                SDFGroupPtr asGroup(SDFNodePtr n);

                std::deque<SDFNodePtr> _nodes;
            };

            /** Handles creating and manipulating a SDFSphere */
            class MakeSphere : public MakeBase< MakeSphere >
            {
            public:
                /** Constructor */
                explicit MakeSphere(MakeRoot *r);

                /** Set radius */
                MakeSphere &radius(float r);

                /** Create node */
                SDFNodePtr createNode() const;

            private:
                float _radius;
            };

            /** Handles creating and manipulating a SDFPlane */
            class MakePlane : public MakeBase< MakePlane >
            {
            public:
                /** Constructor. Initializes a xy-plane. */
                explicit MakePlane(MakeRoot *r);

                /** Set normal of plane */
                MakePlane &normal(const Vector &n);

                /** Create node */
                SDFNodePtr createNode() const;

            private:
                Vector _n;
            };

            /** Handles creating and manipulating a SDFBox */
            class MakeBox : public MakeBase< MakeBox >
            {
            public:
                /** Constructor. Initializes a unit cube. */
                explicit MakeBox(MakeRoot *r);

                /** Set half lengths box will be centered around. */
                MakeBox &halfLengths(const Vector &hl);

                /** Set lengths for box. Box will be centered around origin. */
                MakeBox &lengths(const Vector &hl);

                /** Create node */
                SDFNodePtr createNode() const;

            private:
                Vector _hl;
            };

            /** Handles creating and manipulating a SDFUnion */
            class MakeJoin : public MakeBase< MakeJoin >
            {
            public:
                /** Constructor */
                explicit MakeJoin(MakeRoot *r);

                /** Create node */
                SDFNodePtr createNode() const;
            };


            /** Handles creating and manipulating a SDFIntersection */
            class MakeIntersection : public MakeBase< MakeIntersection >
            {
            public:
                /** Constructor */
                explicit MakeIntersection(MakeRoot *r);

                /** Create node */
                SDFNodePtr createNode() const;
            };

            /** Handles creating and manipulating a SDFDifference */
            class MakeDifference : public MakeBase< MakeDifference >
            {
            public:
                /** Constructor */
                explicit MakeDifference(MakeRoot *r);

                /** Create node */
                SDFNodePtr createNode() const;
            };

            /** Handles creating and manipulating a SDFRigidTransform */
            class MakeTransform : public MakeBase< MakeTransform >
            {
            public:

                /** Constructor. Initialize with identity. */
                explicit MakeTransform(MakeRoot *r);

                /** Set transform */
                MakeTransform &transform(const AffineTransform &t);

                /** Set matrix of transform. */
                MakeTransform &matrix(const AffineTransform::MatrixType &m);

                /** Create node */
                SDFNodePtr createNode() const;

                /** Concat rotation. Supported types are the same as in Eigen. */
                template<class RotationType>
                MakeTransform &rotate(const RotationType &r) {
                    _t.rotate(r);
                    return *this;
                }

                /** Concat translation. Supported types are the same as in Eigen. */
                template<class TranslationType>
                MakeTransform &translate(const TranslationType &t) {
                    _t.translate(t);
                    return *this;
                }
                
            private:
                AffineTransform _t;
            };

            /** Handles creating and manipulating a SDFRepetition */
            class MakeRepetition : public MakeBase< MakeRepetition >
            {
            public:
                /** Constructor. Initializes a repetition that does nothing. */
                explicit MakeRepetition(MakeRoot *r);

                /** Set cell sizes */
                MakeRepetition &cellSizes(const Vector &cs);

                /** Set repetition along x-axis */
                MakeRepetition &x(Scalar s);

                /** Set repetition along x-axis */
                MakeRepetition &y(Scalar s);

                /** Set repetition along x-axis */
                MakeRepetition &z(Scalar s);

                /** Create node */
                SDFNodePtr createNode() const;

            private:
                Vector _cellSizes;
            };

            // Implementation of MakeBase

            template<class Derived> 
            MakeBase<Derived>::MakeBase(MakeRoot *r)
                :_root(r), _isAttached(false), _storePtr(0)
            {}

            template<class Derived>
            MakeSphere MakeBase<Derived>::sphere() {
                deferredAttachNode();
                return MakeSphere(_root);
            }

            template<class Derived>
            MakeBox MakeBase<Derived>::box() {
                deferredAttachNode();
                return MakeBox(_root);
            }

            template<class Derived>
            MakePlane MakeBase<Derived>::plane() {
                deferredAttachNode();
                return MakePlane(_root);
            }

            template<class Derived>
            MakeJoin MakeBase<Derived>::join() {
                deferredAttachNode();
                return MakeJoin(_root);
            }

            template<class Derived>
            MakeIntersection MakeBase<Derived>::intersection() {
                deferredAttachNode();
                return MakeIntersection(_root);
            }

            template<class Derived>
            MakeDifference MakeBase<Derived>::difference() {
                deferredAttachNode();
                return MakeDifference(_root);
            }

            template<class Derived> 
            MakeTransform MakeBase<Derived>::transform() {
                deferredAttachNode();
                return MakeTransform(_root);
            }

            template<class Derived> 
            MakeRepetition MakeBase<Derived>::repetition() {
                deferredAttachNode();
                return MakeRepetition(_root);
            }

            template<class Derived> 
            Derived &MakeBase<Derived>::end() {
                // Attach current node before walking up the tree
                deferredAttachNode(); 
                _root->bubbleUp();
                return *static_cast<Derived*>(this);
            }

            template<class Derived> 
            Derived &MakeBase<Derived>::storeNodePtr(SDFNodePtr *ptr) {
                _storePtr = ptr;
                return *static_cast<Derived*>(this);
            }

            template<class Derived> 
            Derived & MakeBase<Derived>::attach(const std::string &key, SDFNodeAttachmentPtr attachment)
            {
                _attachments[key] = attachment;
                return *static_cast<Derived*>(this);
            }

            template<class Derived> 
            MakeBase<Derived>::operator SDFNodePtr()
            {
                deferredAttachNode();
                return _root->node();
            }		            

            template<class Derived> 
            void MakeBase<Derived>::deferredAttachNode() {
                if (!_isAttached) {
                    SDFNodePtr ptr = static_cast<Derived*>(this)->createNode();
                    if (ptr) // for root
                        ptr->setAttachments(_attachments);
                    _root->addNode(ptr);
                    _isAttached = true;

                    // Provide outside access to node if required.
                    if (_storePtr)
                        *_storePtr = ptr;                    
                }
            }
        }

        /** Start creating a new scene. */
        detail::MakeRoot create();

    }

}

#endif
