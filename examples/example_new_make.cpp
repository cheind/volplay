// This file is part of volplay, a library for interacting with volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include "catch.hpp"
#include <volplay/sdf_make.h>
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_plane.h>
#include <volplay/sdf_box.h>
#include <volplay/sdf_union.h>
#include <volplay/sdf_intersection.h>
#include <volplay/sdf_difference.h>
#include <volplay/sdf_repetition.h>
#include <volplay/sdf_rigid_transform.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#include <volplay/surface/dual_contouring.h>
#include <volplay/surface/off_export.h>
#include <stack>

namespace volplay {
	namespace make {

		class MakeRoot;
		class MakeSphere;
		class MakeJoin;
		class MakeTransform;
		
		template<class Derived>
		class MakeBase {
		public:
			typedef MakeBase<Derived> MakeBaseType;

			MakeSphere sphere();
			MakeJoin join();
			MakeTransform transform();
			Derived &end();
			operator SDFNodePtr();
		protected:
			MakeBase(MakeRoot *r);
			void deferredAttachNode();
			void log(char * format, ...) const;

			MakeRoot *_root;
			bool _isAttached;
		};

		class MakeSphere : public MakeBase< MakeSphere >
		{
		public:

			explicit MakeSphere(MakeRoot *r)
				: MakeBaseType(r), _radius(1)
			{}

			MakeSphere &radius(float r)
			{
				_radius = r;
				return *this;
			}

			SDFNodePtr createNode() const
			{
				return std::make_shared<SDFSphere>(_radius);
			}

		private:
			float _radius;
		};

		class MakeJoin : public MakeBase< MakeJoin >
		{
		public:

			explicit MakeJoin(MakeRoot *r)
				: MakeBaseType(r)
			{}

			SDFNodePtr createNode() const
			{
				return std::make_shared<SDFUnion>();
			}
		};

		class MakeTransform : public MakeBase< MakeTransform >
		{
		public:

			explicit MakeTransform(MakeRoot *r)
				: MakeBaseType(r)
			{
				_t.setIdentity();
			}

			MakeTransform &transform(const AffineTransform &t) {
				_t = t;
				return *this;
			}

			MakeTransform &matrix(const AffineTransform::MatrixType &m) {
				_t.matrix() = m;
				return *this;
			}

			template<class RotationType>
			MakeTransform &rotate(const RotationType &r) {
				_t.rotate(r);
				return *this;
			}

			template<class TranslationType>
			MakeTransform &translate(const TranslationType &t) {
				_t.translate(t);
				return *this;
			}

			SDFNodePtr createNode() const
			{
				return std::make_shared<SDFRigidTransform>(_t);
			}

		private:
			AffineTransform _t;
		};
		
		class MakeRoot : public MakeBase< MakeRoot >
		{
		public:
			MakeRoot()
				: MakeBaseType(this)
			{}

			SDFNodePtr node() const
			{
				return _nodes.back();
			}

			void addNode(SDFNodePtr n)
			{
				if (!n)
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

			void bubbleUp() {
				if (_nodes.size() <= 1) {
					return;
				}

				do {
					_nodes.erase(_nodes.begin());
				} while (_nodes.size() > 1 && !(asGroup(_nodes.front())));
			}

			SDFNodePtr createNode() const
			{
				return SDFNodePtr();
			}

		private:

			SDFGroupPtr asGroup(SDFNodePtr n)
			{
				return std::dynamic_pointer_cast<SDFGroup>(n);
			}

			std::deque<SDFNodePtr> _nodes;
		};

        // Implementation of MakeBase

        template<class Derived>
        MakeSphere MakeBase<Derived>::sphere() {
	        deferredAttachNode();
			return MakeSphere(_root);
        }

        template<class Derived>
		MakeJoin MakeBase<Derived>::join() {
			deferredAttachNode();
			return MakeJoin(_root);
		}

		template<class Derived> 
        MakeTransform MakeBase<Derived>::transform() {
			deferredAttachNode();
			return MakeTransform(_root);
		}

        template<class Derived> 
		Derived &MakeBase<Derived>::end() {
			// Attach current node before walking up the tree
			deferredAttachNode(); 
			_root->bubbleUp();
			return *static_cast<Derived*>(this);
		}

        template<class Derived> 
		MakeBase<Derived>::operator SDFNodePtr()
		{
			deferredAttachNode();
			return _root->node();
		}		

        template<class Derived> 
		MakeBase<Derived>::MakeBase(MakeRoot *r)
			:_root(r), _isAttached(false)
		{}

        template<class Derived> 
		void MakeBase<Derived>::deferredAttachNode() {
			if (!_isAttached) {
				_root->addNode(static_cast<Derived*>(this)->createNode());
				_isAttached = true;
			}
		}

        template<class Derived>
		void MakeBase<Derived>::log(char * format, ...) const
		{
			va_list argptr;
			va_start(argptr, format);					
			vfprintf(stderr, format, argptr);
			va_end(argptr);				
		}

		MakeRoot create()
		{
			return MakeRoot();
		}

	}
}



TEST_CASE("New Make")
{
	namespace vm = volplay::make;
	volplay::SDFNodePtr s = vm::create()
		.join()
			.sphere().radius(0.5)
			.transform().translate(volplay::Vector::Constant(0.5))
				.sphere().radius(0.5)
			.end()
		.end();

	volplay::surface::DualContouring dc;
	volplay::surface::IndexedSurface surface = dc.extractSurface(s, volplay::Vector(-2, -2, -2), volplay::Vector(2, 2, 2), volplay::Vector::Constant(0.25f));
	volplay::surface::OFFExport off;
	off.exportSurface("mysurface.off", surface);

}