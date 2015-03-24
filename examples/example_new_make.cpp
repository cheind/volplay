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

			SDFMaker &transform(const Vector t) {

				AffineTransform at = AffineTransform::Identity();
				at.translate(t);

				SDFRigidTransformPtr n = std::make_shared<SDFRigidTransform>(at);
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

	}
}



TEST_CASE("New Make")
{
	namespace vm = volplay::make;
	volplay::SDFNodePtr scene = vm::create()
		.differenceOf()
			.sphere(vm::SphereOpts().radius(0.5))
			.repetition(/*vm::RepetitionOpts().resX(0.3).resZ(0.3)*/)
				.sphere(vm::SphereOpts().radius(0.2))
			.end()
		.end();

	volplay::surface::DualContouring dc;
	volplay::surface::IndexedSurface surface = dc.extractSurface(scene, volplay::Vector(-2, -2, -2), volplay::Vector(2, 2, 2), volplay::Vector::Constant(0.05f));
	volplay::surface::OFFExport off;
	off.exportSurface("mysurface.off", surface);


}