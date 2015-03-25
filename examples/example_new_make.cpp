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

		class SDFMaker;

		class SphereOpts {
		public:
			SphereOpts()
				:_radius(1)
			{}

			SphereOpts &radius(Scalar r) {
				_radius = r;
				return *this;
			}

		private:
			friend class SDFMaker;
			Scalar _radius;
		};


		class PlaneOpts {
		public:
			PlaneOpts()
				:_normal(0,0,1)
			{}

			PlaneOpts &normal(const Vector n) {
				_normal = n;
				return *this;
			}

			PlaneOpts &normal(Scalar x, Scalar y, Scalar z) {
				_normal = Vector(x, y, z);
				return *this;
			}

		private:
			friend class SDFMaker;

			Vector _normal;
		};

		class RepetitionOpts {
		public:
			RepetitionOpts()
				:_res(Vector::Constant(std::numeric_limits<Scalar>::infinity()))
			{}

			RepetitionOpts &resolution(const Vector r) {
				_res = r;
				return *this;
			}

			RepetitionOpts &resX(Scalar r) {
				_res.x() = r;
				return *this;
			}

			RepetitionOpts &resY(Scalar r) {
				_res.y() = r;
				return *this;
			}

			RepetitionOpts &resZ(Scalar r) {
				_res.z() = r;
				return *this;
			}

		private:
			friend class SDFMaker;

			Vector _res;
		};

		class NodeOpts {
		public:
			NodeOpts()
			{}

			NodeOpts(SDFNodePtr n)
				:_n(n)
			{}

			NodeOpts &node(SDFNodePtr n) {
				_n = n;
				return *this;
			}


		private:
			friend class SDFMaker;

			SDFNodePtr _n;
		};


		class TransformOpts {
		public:
			TransformOpts()
			{
				_t.setIdentity();
			}

			TransformOpts &transform(const AffineTransform &t) {
				_t = t;
				return *this;
			}

			TransformOpts &matrix(const AffineTransform::MatrixType &m) {
				_t.matrix() = m;				
				return *this;
			}

			template<class RotationType>
			TransformOpts &rotate(const RotationType &r) {
				_t.rotate(r);
				return *this;
			}

			template<class TranslationType>
			TransformOpts &translate(const TranslationType &t) {
				_t.translate(t);
				return *this;
			}

		private:
			friend class SDFMaker;

			AffineTransform _t;
		};

		class SDFMaker {
		public:

			SDFMaker(bool verbose)
				:_verbose(verbose)
			{}
		
			SDFMaker &unionOf() {
				SDFNodePtr n = std::make_shared<SDFUnion>();
				add(n);
				return *this;
			}

			SDFMaker &intersectionOf() {
				SDFNodePtr n = std::make_shared<SDFIntersection>();
				add(n);
				return *this;
			}

			SDFMaker &differenceOf() {
				SDFNodePtr n = std::make_shared<SDFDifference>();
				add(n);
				return *this;
			}

			SDFMaker &node(NodeOpts opts) {			
				logIf(!opts._n, "Cannot add null-node.");
				if (opts._n)
					add(opts._n);
				return *this;
			}

			SDFMaker &plane(PlaneOpts opts = PlaneOpts()) {
				SDFPlanePtr n = std::make_shared<SDFPlane>(opts._normal);
				add(n);
				return *this;
			}

			SDFMaker &sphere(SphereOpts opts = SphereOpts()) {
				SDFSpherePtr n = std::make_shared<SDFSphere>(opts._radius);
				add(n);
				return *this;
			}

			SDFMaker &transform(TransformOpts opts = TransformOpts()) {
				
				SDFRigidTransformPtr n = std::make_shared<SDFRigidTransform>(opts._t);
				add(n);

				return *this;
			}

			SDFMaker &repetition(RepetitionOpts opts = RepetitionOpts()) {
				SDFRepetitionPtr n = std::make_shared<SDFRepetition>(opts._res);
				add(n);
				return *this;
			}

			SDFMaker &end() {				
				if (_nodes.size() <= 1) {
					return *this;
				}

				do {
					_nodes.erase(_nodes.begin());
				} while (_nodes.size() > 1 && !(asGroup(_nodes.front()) || asUnary(_nodes.front())));

				return *this;
			}

			operator SDFNodePtr() const
			{
				if (_nodes.empty())
					return SDFNodePtr();

				return _nodes.back();
			}

		private:
			void add(SDFNodePtr n)
			{
				const bool needsPush = asGroup(n) || asUnary(n);

				if (_nodes.empty()) {
					// Top node is always pushed
					_nodes.push_front(n);
				}
				else if (SDFGroupPtr g = asGroup(_nodes.front())) {
					g->add(n);
					if (needsPush) 
						_nodes.push_front(n);
				}
				else if (SDFUnaryPtr u = asUnary(_nodes.front())) {	
					logIf((bool)u->node(), "Warning node already set at unary function. Value will be overridden");
					u->setNode(n);
					if (needsPush)
						_nodes.push_front(n);
				}
			}

			SDFGroupPtr asGroup(SDFNodePtr n)
			{
				return std::dynamic_pointer_cast<SDFGroup>(n);
			}

			SDFUnaryPtr asUnary(SDFNodePtr n)
			{
				return std::dynamic_pointer_cast<SDFUnary>(n);
			}

			void logIf(bool condition, const char *message) {
				if (condition && _verbose)
					std::cout << "SDFMaker: " << message << std::endl;
			}

			std::deque<SDFNodePtr> _nodes;
			bool _verbose;
		};

		SDFMaker create()
		{
			return SDFMaker(true);
		}

		struct NoParent {
			void addNode(SDFNodePtr n){};
			SDFNodePtr createNode() const {};
		};

		class MakeRoot;
		template<class Previous> class MakeSphere;
		template<class Previous> class MakeJoin;
		template<class Previous> class MakeTransform;
		
		template<class Derived>
		class MakeBase {
		public:

			typedef MakeBase<Derived> MakeBaseType;

			MakeSphere< MakeBaseType > sphere() {
				deferredAttachNode();
				return MakeSphere< MakeBaseType >(_root);
			}

			MakeJoin< MakeBaseType > join() {
				deferredAttachNode();
				return MakeJoin< MakeBaseType >(_root);
			}

			MakeTransform< MakeBaseType > transform() {
				deferredAttachNode();
				return MakeTransform< MakeBaseType >(_root);
			}

			Derived &end() {
				// Attach current node before walking up the tree
				deferredAttachNode(); 
				_root->bubbleUp();

				return *static_cast<Derived*>(this);
			}

			operator SDFNodePtr()
			{
				deferredAttachNode();
				return _root->node();
			}		

		protected:

			MakeBase(MakeRoot *r)
				:_root(r), _isAttached(false)
			{}

			void deferredAttachNode() {
				if (!_isAttached) {
					_root->addNode(static_cast<Derived*>(this)->createNode());
					_isAttached = true;
				}
			}

			void log(char * format, ...) const
			{
				va_list argptr;
				va_start(argptr, format);					
				vfprintf(stderr, format, argptr);
				va_end(argptr);				
			}

			MakeRoot *_root;
			bool _isAttached;
		};

		template<class Previous>
		class MakeSphere : public MakeBase< MakeSphere<Previous> >
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

		template<class Previous>
		class MakeJoin : public MakeBase< MakeJoin<Previous> >
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

		template<class Previous>
		class MakeTransform : public MakeBase< MakeTransform<Previous> >
		{
		public:

			explicit MakeTransform(MakeRoot *r)
				: MakeBaseType(r)
			{
				_t.setIdentity();
			}

			MakeTransform<Previous> &transform(const AffineTransform &t) {
				_t = t;
				return *this;
			}

			MakeTransform<Previous> &matrix(const AffineTransform::MatrixType &m) {
				_t.matrix() = m;
				return *this;
			}

			template<class RotationType>
			MakeTransform<Previous> &rotate(const RotationType &r) {
				_t.rotate(r);
				return *this;
			}

			template<class TranslationType>
			MakeTransform<Previous> &translate(const TranslationType &t) {
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
			MakeRoot(bool verbose)
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

				const bool needsPush = (bool)(asGroup(n) || asUnary(n));

				if (_nodes.empty()) {
					// Top node is always pushed					
					_nodes.push_front(n);
				}
				else if (SDFGroupPtr g = asGroup(_nodes.front())) {
					g->add(n);
					if (needsPush)
						_nodes.push_front(n);
				}
				else if (SDFUnaryPtr u = asUnary(_nodes.front())) {
					u->setNode(n);
					if (needsPush)
						_nodes.push_front(n);
				}
			}

			void bubbleUp() {
				if (_nodes.size() <= 1) {
					return;
				}

				do {
					_nodes.erase(_nodes.begin());
				} while (_nodes.size() > 1 && !(asGroup(_nodes.front()) || asUnary(_nodes.front())));
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

			SDFUnaryPtr asUnary(SDFNodePtr n)
			{
				return std::dynamic_pointer_cast<SDFUnary>(n);
			}

			std::deque<SDFNodePtr> _nodes;
		};

	}
}



TEST_CASE("New Make")
{
	/*
	namespace vm = volplay::make;
	volplay::SDFNodePtr scene1 = vm::create()
		.repetition(vm::RepetitionOpts().resX(1).resY(1).resZ(1))
			.differenceOf()
				.sphere(vm::SphereOpts().radius(0.5))
				.transform(vm::TransformOpts().translate(volplay::Vector(0.5, 0.5, 0.5)))
					.sphere(vm::SphereOpts().radius(0.5))
				.end()
			.end()
		.end();

	volplay::SDFNodePtr scene = vm::create()
		.differenceOf()
			.node(vm::NodeOpts().node(scene1)).end() // this is ugly. end is necessary as scene1 is a repetition.
			.plane()
		.end();
		
	volplay::surface::DualContouring dc;
	volplay::surface::IndexedSurface surface = dc.extractSurface(scene, volplay::Vector(-2, -2, -2), volplay::Vector(2, 2, 2), volplay::Vector::Constant(0.025f));
	volplay::surface::OFFExport off;
	off.exportSurface("mysurface.off", surface);
	*/
	
	namespace vm = volplay::make;
	vm::MakeRoot ms(true);
	volplay::SDFNodePtr s = ms.join()
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