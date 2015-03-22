// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include "catch.hpp"
#include "float_comparison.hpp"
#include <volplay/util/voxel_grid.h>
#include <volplay/util/function_output_iterator.h>
#include <iostream>

namespace vp = volplay;
namespace vg = volplay::util::voxelgrid;

TEST_CASE("VoxelGrid-Transforms")
{
    vp::AffineTransform t = vg::buildWorldToLocal(vp::Vector(10, 10, 10), vp::Vector(2.f, 4.f, 8.f));

    Eigen::Matrix<vp::Scalar, Eigen::Dynamic, Eigen::Dynamic> ref(3, 4);
    ref << 0.5f, 0, 0, -5.f, 
           0, 0.25f, 0, -2.5f,
           0, 0, 0.125f, -1.25f;

    REQUIRE_CLOSE_VECTOR(ref, t.matrix());
    REQUIRE_CLOSE_VECTOR(vg::worldToLocal(t, vp::Vector(10, 10, 10)), vp::Vector(0,0,0));
    REQUIRE_CLOSE_VECTOR(vg::worldToLocal(t, vp::Vector(11, 15, 20)), vp::Vector(0.5f,1.25f,1.25f));
    REQUIRE_CLOSE_VECTOR(vg::worldToLocal(t, vp::Vector(0, -1, -2)), vp::Vector(-5.f,-2.75f,-1.5f));
    REQUIRE_CLOSE_VECTOR(vg::worldToVoxel(t, vp::Vector(10, 10, 10)), vp::Index(0,0,0));
    REQUIRE_CLOSE_VECTOR(vg::worldToVoxel(t, vp::Vector(11, 15, 20)), vp::Index(0,1,1));
    REQUIRE_CLOSE_VECTOR(vg::worldToVoxel(t, vp::Vector(0, -1, -2)), vp::Index(-5,-3,-2));
}


TEST_CASE("VoxelGrid-Edge-Orientation")
{
    REQUIRE(vg::flipEdge(vg::VoxelEdge(vg::Voxel(0,0,0), vg::Voxel(1, 0, 0))) == 
            vg::VoxelEdge(vg::Voxel(1,0,0), vg::Voxel(0, 0, 0)));

    REQUIRE(vg::undirectedEdge(vg::VoxelEdge(vg::Voxel(0,0,0), vg::Voxel(1, 0, 0))) == 
            vg::VoxelEdge(vg::Voxel(0,0,0), vg::Voxel(1, 0, 0)));

    REQUIRE(vg::undirectedEdge(vg::VoxelEdge(vg::Voxel(0,0,2), vg::Voxel(0, 0, 1))) == 
            vg::VoxelEdge(vg::Voxel(0,0,1), vg::Voxel(0, 0, 2)));

}

TEST_CASE("VoxelGrid-Voxel-Properties")
{
    vg::SparseVoxelProperty<int> viprop(-1);

    REQUIRE(!viprop.isSet(vp::Index(0,0,0)));
    REQUIRE(viprop[vp::Index(0,0,0)] == -1);
    viprop[vp::Index(0,0,0)] = 5;
    viprop[vp::Index(0,0,0)] = 6;
    REQUIRE(viprop.isSet(vp::Index(0,0,0)));
    REQUIRE(viprop[vp::Index(0,0,0)] == 6);

    REQUIRE(std::distance(viprop.begin(), viprop.end()) == 1);
}

TEST_CASE("VoxelGrid-VoxelEdge-Properties")
{
    vg::SparseVoxelEdgeProperty<int> prop(-1);

    auto e1 = std::make_pair(vp::Index(0,0,0), vp::Index(1,0,0));
    auto e2 = std::make_pair(vp::Index(1,0,0), vp::Index(0,0,0));
    auto e3 = std::make_pair(vp::Index(0,0,0), vp::Index(0,0,1));
    
    REQUIRE(!prop.isSet(e1));
    REQUIRE(prop[e1] == -1);
    prop[e1] = 0;
    prop[e2] = 1;
    prop[e3] = 2;
    
    REQUIRE(prop[e1] == 0);
    REQUIRE(prop[e2] == 1);
    REQUIRE(prop[e3] == 2);

    REQUIRE(std::distance(prop.begin(), prop.end()) == 3);
}

TEST_CASE("VoxelGrid-SparseSets")
{
    vg::SparseVoxelSet vs;
    vg::SparseVoxelEdgeSet es;

    REQUIRE(!vs.isSet(vp::Index(0,0,0)));
    vs.set(vp::Index(0,0,0));
    REQUIRE(vs.isSet(vp::Index(0,0,0)));
    REQUIRE(vs.size() == 1);

    REQUIRE(!es.isSet(vg::VoxelEdge(vp::Index(0,0,0), vp::Index(0,0,1))));
    es.set(vg::VoxelEdge(vp::Index(0,0,0), vp::Index(0,0,1)));
    REQUIRE(es.isSet(vg::VoxelEdge(vp::Index(0,0,0), vp::Index(0,0,1))));
    REQUIRE(!es.isSet(vg::VoxelEdge(vp::Index(0,0,1), vp::Index(0,0,0))));
    REQUIRE(es.size() == 1);
}

TEST_CASE("VoxelGrid-EdgeQuery-Canonical")
{
    int count = 0;
    vg::SparseVoxelEdgeProperty<bool> prop(false);

    // Just one cube, should give 12 edges
    vg::edges(vp::util::makeFunctionOutputIterator([&](const vg::VoxelEdge &e)
    {
        prop[e] = true;
        ++count;
    }));

    REQUIRE(count == 12);
    REQUIRE(prop.size() == 12);

    for (auto iter = prop.begin(); iter != prop.end(); ++iter)
    {
        vg::Voxel v = iter->first.second - iter->first.first;
        REQUIRE(v.maxCoeff() == 1); 
        REQUIRE(v.minCoeff() == 0); 
        REQUIRE(v.sum() == 1);
    }
}

TEST_CASE("VoxelGrid-EdgeQuery-Offset")
{
    int count = 0;
    vg::SparseVoxelEdgeProperty<bool> prop(false);

    // Just one cube, should give 12 edges
    vg::edges(vg::Voxel(5,5,5), vp::util::makeFunctionOutputIterator([&](const vg::VoxelEdge &e)
    {
        prop[e] = true;
        ++count;
    }));

    REQUIRE(count == 12);
    REQUIRE(prop.size() == 12);

    for (auto iter = prop.begin(); iter != prop.end(); ++iter)
    {
        vg::Voxel v = iter->first.second - iter->first.first;
        REQUIRE(v.maxCoeff() == 1); 
        REQUIRE(v.minCoeff() == 0); 
        REQUIRE(v.sum() == 1);
        REQUIRE((iter->first.first - vg::Voxel(5,5,5)).minCoeff() == 0);
    }
}

TEST_CASE("VoxelGrid-EdgeQuery-InRange")
{
    int count = 0;
    vg::SparseVoxelEdgeProperty<bool> prop(false);

    // Just one cube, should give 12 edges
    vg::edges(vg::Voxel(5,5,5), vg::Voxel(6,6,6), vp::util::makeFunctionOutputIterator([&](const vg::VoxelEdge &e)
    {
        prop[e] = true;
        ++count;
    }));
    
    REQUIRE(count == 12);
    REQUIRE(prop.size() == 12);
    
    vg::VoxelEdge edges[12];
    vg::edges(vg::Voxel(5,5,5), edges);

    for (size_t i = 0; i < 12; ++i) {
        REQUIRE(prop.isSet(edges[i]));
    }

    vg::edges(vg::Voxel(6,6,6), edges);
    for (size_t i = 0; i < 12; ++i) {
        REQUIRE(!prop.isSet(edges[i]));
    }
}

TEST_CASE("VoxelGrid-VoxelQuery-Edge")
{
    vg::Voxel v[4];
    vg::voxels(vg::VoxelEdge(vg::Voxel(5,5,5), vg::Voxel(5,5,6)), v);

    REQUIRE(v[0] == vg::Voxel(5,5,5));
    REQUIRE(v[1] == vg::Voxel(4,5,5));
    REQUIRE(v[2] == vg::Voxel(4,4,5));
    REQUIRE(v[3] == vg::Voxel(5,4,5));


    vg::voxels(vg::VoxelEdge(vg::Voxel(0,0,0), vg::Voxel(1,0,0)), v);
    REQUIRE(v[0] == vg::Voxel(0,0,0));
    REQUIRE(v[1] == vg::Voxel(0,-1,0));
    REQUIRE(v[2] == vg::Voxel(0,-1,-1));
    REQUIRE(v[3] == vg::Voxel(0,0,-1));

    vg::voxels(vg::VoxelEdge(vg::Voxel(1,0,0), vg::Voxel(0,0,0)), v);
    REQUIRE(v[3] == vg::Voxel(0,0,0));
    REQUIRE(v[2] == vg::Voxel(0,-1,0));
    REQUIRE(v[1] == vg::Voxel(0,-1,-1));
    REQUIRE(v[0] == vg::Voxel(0,0,-1));
}