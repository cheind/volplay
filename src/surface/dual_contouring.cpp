// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/surface/dual_contouring.h>
#include <volplay/sdf_node.h>
#include <volplay/util/function_output_iterator.h>
#include <volplay/util/voxel_grid.h>
#include <volplay/math/sign.h>
#include <iostream>

namespace volplay {
    
    namespace surface {

        struct Hermite {
            Vector p;
            Vector n;

            Hermite()
                :p(3), n(3)
            {}
        };



        DualContouring::DualContouring()
        {}

        IndexedSurface
        DualContouring::extractSurface(SDFNodePtr scene, const Vector &lower, const Vector &upper, const Vector &resolution)
        {
            namespace vg = util::voxelgrid;

            IndexedSurface surface;

            AffineTransform toVG = vg::buildWorldToLocal(lower, resolution);
            AffineTransform toWorld = toVG.inverse();
            
            // 1. Iterate all edges and remember those with an surface itersection. 
            // For each such edge record the intersection data.
            
            vg::SparseVoxelSet voxels;
            vg::SparseVoxelEdgeProperty<Hermite> eHermite;
            vg::edges(vg::worldToVoxel(toVG, lower), vg::worldToVoxel(toVG, upper), util::oiter([&](const vg::VoxelEdge &eUndirected) {
               
                Vector verts[2] = {
                    Vector(toWorld * eUndirected.first.cast<Scalar>()),
                    Vector(toWorld * eUndirected.second.cast<Scalar>())
                };

                Scalar sdf[2] = {
                    scene->eval(verts[0]),
                    scene->eval(verts[1])
                };

                int sgn[2] = {
                    math::sign(sdf[0]),
                    math::sign(sdf[1])
                };

                if ((sgn[0] - sgn[1]) == 0)
                    return;

                // Orient edge so intersection points outward.
                util::voxelgrid::VoxelEdge e;
                if (sgn[0] < sgn[1]) {
                    e = eUndirected;
                } else {
                    e = util::voxelgrid::flipEdge(eUndirected);
                    std::swap(verts[0], verts[1]);
                    std::swap(sdf[0], sdf[1]);
                    std::swap(sgn[0], sgn[1]);
                }
                
                // Determine hermite data. Simply use secant method here with one iteration
                Vector v = verts[1] - verts[0];
                Scalar t = 1.f - sdf[1]  * ((1.f) / (sdf[1] - sdf[0]));
                if (t == Scalar(1)) // Exclude intersections on corners of other voxels.
                    return;
                
                Hermite &h = eHermite[e];
                h.p = verts[0] + t * v;
                h.n = scene->normal(h.p);

                // Mark surrounding voxels
                util::voxelgrid::voxels(e, util::oiter([&](const util::voxelgrid::Voxel &v) { 
                    voxels.set(v);                        
                }));
            }));

            // 2. Solve for each voxel 

            surface.vertices.resize(3, voxels.size());
			vg::SparseVoxelProperty<vg::Voxel::Index> voxelToIndex(0);
			vg::Voxel::Index count = 0;
            for (auto iter = voxels.begin(); iter != voxels.end(); ++iter, ++count) {

                // Just use voxel mid-point:
                //surface.vertices.col(count) = (toWorld * iter->cast<float>()) + resolution * 0.5f;

                // Solve for plane intersection
                vg::VoxelEdge edges[12];
                vg::edges(*iter, edges);
                
                Vector m = Vector::Zero(3);
                vg::Voxel::Index nActive = 0;
                for (int i = 0; i < 12; ++i) {
                    if (eHermite.isSet(edges[i])) {
                        m += eHermite[edges[i]].p;
                        ++nActive;
                    } else if (eHermite.isSet(vg::flipEdge(edges[i]))) {
                        m += eHermite[vg::flipEdge(edges[i])].p;
                        ++nActive;
                    }
                }
                m /= Scalar(nActive);

				Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> A(nActive, 3);
                Eigen::Matrix<Scalar, Eigen::Dynamic, 1> b(nActive);
               
                nActive = 0;                
                for (int i = 0; i < 12; ++i) {
                    if (eHermite.isSet(edges[i])) {
                        const Hermite &h = eHermite[edges[i]];
                        A.row(nActive) = h.n.transpose();
                        b(nActive) = h.n.dot(h.p - m);
                        ++nActive;

                    } else if (eHermite.isSet(vg::flipEdge(edges[i]))) {
                        const Hermite &h = eHermite[vg::flipEdge(edges[i])];
                        A.row(nActive) = h.n.transpose();
                        b(nActive) = h.n.dot(h.p - m);
                        ++nActive;
                    }
                }

                Eigen::JacobiSVD< Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> > svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
                svd.setThreshold(Scalar(0.1) / svd.singularValues()(0));
                Vector x = svd.solve(b) + m;

                surface.vertices.col(count) = x;
                voxelToIndex[*iter] = count;
                
            }

            // 3. Build topology
            // Actually dual contouring generates quads. We triangulate them however, for compatibility issues
            // with most external 3D viewers.
            
            surface.faces.resize(3, eHermite.size() * 2);
            count = 0;  
            for (auto iter = eHermite.begin(); iter != eHermite.end(); ++iter) {
                vg::Voxel v[4];
                util::voxelgrid::voxels(iter->first, v);

                surface.faces(0, count) = voxelToIndex[v[0]];
                surface.faces(1, count) = voxelToIndex[v[1]];
                surface.faces(2, count) = voxelToIndex[v[2]];
                count += 1;
                surface.faces(0, count) = voxelToIndex[v[0]];
                surface.faces(1, count) = voxelToIndex[v[2]];
                surface.faces(2, count) = voxelToIndex[v[3]];
                count += 1;                
            }
            
            /* Code for Quads
            surface.faces.resize(4, eHermite.size());
            count = 0;  
            for (auto iter = eHermite.begin(); iter != eHermite.end(); ++iter, ++count) {
                vg::Voxel v[4];
                util::voxelgrid::voxels(iter->first, v);
                surface.faces(0, count) = voxelToIndex[v[0]];
                surface.faces(1, count) = voxelToIndex[v[1]];
                surface.faces(2, count) = voxelToIndex[v[2]];
                surface.faces(3, count) = voxelToIndex[v[3]];
            }
            std::cout << "faces done." << std::endl;
            */

            return surface;
        }

    }
    
    
}