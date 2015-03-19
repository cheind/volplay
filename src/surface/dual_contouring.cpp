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

        template <typename T> int sign(T val) {
            return (T(0) < val) - (val < T(0));
        }

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
               
                const Scalar sdf[2] = {
                    scene->eval(toWorld * eUndirected.first.cast<float>()),
                    scene->eval(toWorld * eUndirected.second.cast<float>())
                };

                const int sgn[2] = {
                    sign(sdf[0]),
                    sign(sdf[1])
                };

                if ((sgn[0] - sgn[1]) == 0)
                    return;

                // Orient edge so intersection points outward.
                const util::voxelgrid::VoxelEdge e = (sgn[0] < sgn[1]) ? eUndirected : util::voxelgrid::oppositeEdge(eUndirected);
                eHermite[e] = Hermite();
                // Mark surrounding voxels
                util::voxelgrid::voxels(e, util::oiter([&](const util::voxelgrid::Voxel &v) { 
                    voxels.set(v);                        
                }));

            }));

            std::cout << "edges done." << std::endl;

            // 2. Solve for each voxel 

            surface.vertices.resize(3, voxels.size());
            vg::SparseVoxelProperty<size_t> voxelToIndex(0);
            size_t count = 0;
            for (auto iter = voxels.begin(); iter != voxels.end(); ++iter, ++count) {
                voxelToIndex[*iter] = count;
                surface.vertices.col(count) = (toWorld * iter->cast<float>()) + resolution * 0.5f;
            }

             std::cout << "vertices done." << std::endl;

            // 3. Build topology
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


            return surface;
        }

    }
    
    
}