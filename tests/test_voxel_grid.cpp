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
#include <iostream>

namespace vp = volplay;
namespace vpu = volplay::util;

TEST_CASE("VoxelGrid-Transforms")
{
    vp::AffineTransform t = vpu::VoxelGrid::buildWorldToLocal(vp::Vector(10, 10, 10), vp::Vector(2.f, 4.f, 8.f));

    Eigen::MatrixXf ref(3, 4);
    ref << 0.5f, 0, 0, -5.f, 
           0, 0.25f, 0, -2.5f,
           0, 0, 0.125f, -1.25f;

    REQUIRE_CLOSE_VECTOR(ref, t.matrix());
    REQUIRE_CLOSE_VECTOR(vpu::VoxelGrid::worldToLocal(t, vp::Vector(10, 10, 10)), vp::Vector(0,0,0));
    REQUIRE_CLOSE_VECTOR(vpu::VoxelGrid::worldToLocal(t, vp::Vector(11, 15, 20)), vp::Vector(0.5f,1.25f,1.25f));
    REQUIRE_CLOSE_VECTOR(vpu::VoxelGrid::worldToLocal(t, vp::Vector(0, -1, -2)), vp::Vector(-5.f,-2.75f,-1.5f));
    REQUIRE_CLOSE_VECTOR(vpu::VoxelGrid::worldToVoxel(t, vp::Vector(10, 10, 10)), vp::Index(0,0,0));
    REQUIRE_CLOSE_VECTOR(vpu::VoxelGrid::worldToVoxel(t, vp::Vector(11, 15, 20)), vp::Index(0,1,1));
    REQUIRE_CLOSE_VECTOR(vpu::VoxelGrid::worldToVoxel(t, vp::Vector(0, -1, -2)), vp::Index(-5,-3,-2));
}

TEST_CASE("VoxelGrid-Voxel-Properties")
{
    vpu::SparseVoxelProperty<int> viprop(-1);

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
    vpu::SparseVoxelEdgeProperty<int> prop(-1);

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