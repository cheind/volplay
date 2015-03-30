// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/surface/dual_contouring.h>
#include <volplay/surface/indexed_surface.h>
#include <volplay/sdf_node.h>
#include <volplay/sdf_displacement.h>
#include <volplay/sdf_make.h>
#include <volplay/util/function_output_iterator.h>
#include <volplay/util/voxel_grid.h>
#include <volplay/math/sign.h>
#include <iostream>

namespace volplay {
    
    namespace surface {
        
        DualContouring::DualContouring()
            : _lower(Vector::Constant(S(-1))),
              _upper(Vector::Constant(S(1))),
              _resolution(Vector::Constant(S(0.01))),
              _iso(S(0))
        {}

        void DualContouring::setLowerBounds(const Vector &lower)
        {
            _lower = lower;
        }

        void DualContouring::setUpperBounds(const Vector &upper)
        {
            _upper = upper;
        }

        void DualContouring::setResolution(const Vector &resolution)
        {
            _resolution = resolution;
        }

        void DualContouring::setIsoLevel(Scalar s)
        {
            _iso = s;
        }

        /** Data associated with edges crossed by surface. */
        struct Hermite {
            Vector p;
            Vector n;
            bool needFlip;

            Hermite()
                :p(3), n(3), needFlip(false)
            {}
        };

        /** Describes the world. */
        struct WorldInfo {
            SDFNodePtr scene;
            Vector lower, upper, resolution;

            AffineTransform toGrid;
            AffineTransform toWorld;

            util::voxelgrid::SparseVoxelEdgeProperty<Hermite> eHermite;

            WorldInfo(SDFNodePtr scene_, const Vector &lower_, const Vector &upper_, const Vector &resolution_)
                : scene(scene_), lower(lower_), upper(upper_), resolution(resolution_)
            {
                toGrid = util::voxelgrid::buildWorldToLocal(lower, resolution);
                toWorld = toGrid.inverse();
            }
        };
        
        /** Computes edge itersection using a linear model assumption */
        class EdgeIntersectionLinear {
        public:
            EdgeIntersectionLinear()
            {}

            bool operator()(const util::voxelgrid::VoxelEdge &e, WorldInfo &wi, Hermite &h) const
            {
                // Edge vertices in world space, SDFs and signs
                Vector verts[2] = {Vector(wi.toWorld * e.first.cast<Scalar>()), Vector(wi.toWorld * e.second.cast<Scalar>())};
                Scalar sdf[2] = { wi.scene->eval(verts[0]), wi.scene->eval(verts[1]) };
                int sgn[2] = { math::sign(sdf[0]), math::sign(sdf[1]) };

                if ((sgn[0] - sgn[1]) == 0)
                    return false;

                bool needFlip = false;
                if (sgn[0] >= sgn[1]) {                    
                    std::swap(verts[0], verts[1]);
                    std::swap(sdf[0], sdf[1]);
                    needFlip = true;
                } 
                
                // Determine hermite data. Under the linear assumption a single step of the secant method
                // should bring us to the root.
                Vector v = verts[1] - verts[0];
                Scalar t = 1.f - sdf[1]  * ((1.f) / (sdf[1] - sdf[0]));
                
                if (t == Scalar(1)) // Exclude intersections on corners of other voxels.
                    return false;
                                
                h.needFlip = needFlip;
                h.p = verts[0] + t * v;
                h.n = wi.scene->normal(h.p);

                return true;
            }
        };

        /** Vertex placement by minimizing the quadric error function QEF as described in Dual Contouring of Hermite data */
        class VertexPlacementDC {
        public:
            void operator()(const util::voxelgrid::Voxel &v, WorldInfo &wi, Vector &x) const
            {
                // Solve for plane intersection
                util::voxelgrid::VoxelEdge edges[12];
                util::voxelgrid::edges(v, edges);
                
                // Determine number of planes and mass point
                Vector m = Vector::Zero(3);
                util::voxelgrid::Voxel::Index nActive = 0;
                for (int i = 0; i < 12; ++i) {
                    if (wi.eHermite.isSet(edges[i])) {
                        m += wi.eHermite[edges[i]].p;
                        ++nActive;
                    }
                }
                m /= Scalar(nActive);

                // Build A.x = b, where b is being translated towards the mass point origin
				Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> A(nActive, 3);
                Eigen::Matrix<Scalar, Eigen::Dynamic, 1> b(nActive);
               
                nActive = 0;                
                for (int i = 0; i < 12; ++i) {
                    if (wi.eHermite.isSet(edges[i])) {
                        const Hermite &h = wi.eHermite[edges[i]];
                        A.row(nActive) = h.n.transpose();
                        b(nActive) = h.n.dot(h.p - m);
                        ++nActive;
                    } 
                }

                // Solve
                Eigen::JacobiSVD< Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> > svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
                svd.setThreshold(Scalar(0.1) / svd.singularValues()(0));
                x = svd.solve(b) + m;
            }
        };

        /** Vertex at midpoint of cell. */
        class VertexPlacementMidpoint {
        public:
            void operator()(const util::voxelgrid::Voxel &v, WorldInfo &wi, Vector &x) const
            {
                x = (wi.toWorld * v.cast<Scalar>()) + wi.resolution * Scalar(0.5);
            }
        };

        /** Actual computation method */
        template<class EdgeIntersectionFnc, class VertexPlacementFnc>
        IndexedSurface
        computeSurface(
            WorldInfo &wi,
            EdgeIntersectionFnc eisect,
            VertexPlacementFnc vplace)
        {
            namespace vg = util::voxelgrid;

            // Iterate all edges and remember those with an surface itersection. 
            // For each such edge record the intersection (Hermite) edge data and
            // mark surrounding voxels to contain vertices.
            
            vg::SparseVoxelSet voxelsWithVertices;
            vg::edges(vg::worldToVoxel(wi.toGrid, wi.lower), vg::worldToVoxel(wi.toGrid, wi.upper), util::oiter([&](const vg::VoxelEdge &e) {
                
                Hermite h;               
                if (eisect(e, wi, h)) {
                    wi.eHermite[e] = h;

                    // Mark surrounding voxels
                    util::voxelgrid::Voxel voxels[4];
                    util::voxelgrid::voxels(e, voxels);

                    voxelsWithVertices.set(voxels[0]);
                    voxelsWithVertices.set(voxels[1]);
                    voxelsWithVertices.set(voxels[2]);
                    voxelsWithVertices.set(voxels[3]);
                }

            }));

            // Solve for each marked voxel from the previous step
            IndexedSurface surface;
            surface.vertices.resize(3, voxelsWithVertices.size());

			vg::SparseVoxelProperty<vg::Voxel::Index> voxelToIndex(0);
			vg::Voxel::Index count = 0;
            for (auto iter = voxelsWithVertices.begin(); iter != voxelsWithVertices.end(); ++iter) {

                Vector x;
                vplace(*iter, wi, x);
                surface.vertices.col(count) = x;
                voxelToIndex[*iter] = count++;                
            }

            // Build topology
            // Actually dual contouring generates quads. We triangulate them however, for compatibility issues
            // with most external 3D viewers.
            
            surface.faces.resize(3, wi.eHermite.size() * 2);
            count = 0;  
            for (auto iter = wi.eHermite.begin(); iter != wi.eHermite.end(); ++iter) {
                vg::Voxel v[4];                
                util::voxelgrid::voxels((iter->second.needFlip ? vg::flipEdge(iter->first) : iter->first), v);

                surface.faces(0, count) = voxelToIndex[v[0]];
                surface.faces(1, count) = voxelToIndex[v[1]];
                surface.faces(2, count) = voxelToIndex[v[2]];
                count += 1;
                surface.faces(0, count) = voxelToIndex[v[0]];
                surface.faces(1, count) = voxelToIndex[v[2]];
                surface.faces(2, count) = voxelToIndex[v[3]];
                count += 1;                
            }

            return surface;
            
            /* Code for Quads
            surface.faces.resize(4, wi.eHermite.size());
            count = 0;  
            for (auto iter = wi.eHermite.begin(); iter != wi.eHermite.end(); ++iter, ++count) {
                vg::Voxel v[4];
                util::voxelgrid::voxels(iter->first, v);
                surface.faces(0, count) = voxelToIndex[v[0]];
                surface.faces(1, count) = voxelToIndex[v[1]];
                surface.faces(2, count) = voxelToIndex[v[2]];
                surface.faces(3, count) = voxelToIndex[v[3]];
            }
            std::cout << "faces done." << std::endl;
            */
        }

        IndexedSurface
        DualContouring::compute(SDFNodePtr scene, EComputeType et)
        {
            WorldInfo wi(scene, _lower, _upper, _resolution);

            // If iso-level is other then zero, we simply offset the entire scene
            // by the constant negative iso value.
            if (_iso != S(0)) {
                wi.scene = make()
                    .displacement().fnc([this](const Vector &v) -> S { return -_iso;})
                        .wrap().node(wi.scene)
                    .end();   
            }

            switch (et) {
            case COMPUTE_LINEAR_DC:
                return computeSurface(wi, EdgeIntersectionLinear(), VertexPlacementDC());
            case COMPUTE_MIDPOINT:
                return computeSurface(wi, EdgeIntersectionLinear(), VertexPlacementMidpoint());
            default:
                return IndexedSurface();
            }
        }

    }
    
    
}