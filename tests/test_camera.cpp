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
#include <volplay/rendering/camera.h>

namespace vp = volplay;

TEST_CASE("Camera projection")
{
    vp::rendering::Camera c;
    vp::AffineTransform t = vp::AffineTransform::Identity();
    t.translate(vp::Vector(10, 10, 10));
    
    c.setCameraToImage(vp::Vector2(100, 100), vp::Vector2(320, 240));
    c.setCameraToWorld(t);
    
    vp::rendering::Camera::Matrix34 w2c = c.worldToCamera();
    vp::rendering::Camera::Matrix34 w2i = c.worldToImage();
    
    vp::Vector x = w2c * vp::Vector(0,0,50).homogeneous();
    
    REQUIRE_CLOSE(x(0), -10);
    REQUIRE_CLOSE(x(1), -10);
    REQUIRE_CLOSE(x(2), 40);
    
    x = w2i * vp::Vector(0,0,50).homogeneous();
    
    REQUIRE_CLOSE(x(0), 100 * -10 + 320 * 40);
    REQUIRE_CLOSE(x(1), 100 * -10 + 240 * 40);
    REQUIRE_CLOSE(x(2), 40);
    
    vp::Vector2 i = x.hnormalized();
    REQUIRE_CLOSE(i(0), (100 * -10 + 320 * 40) / vp::Scalar(40));
    REQUIRE_CLOSE(i(1), (100 * -10 + 240 * 40) / vp::Scalar(40));
}

TEST_CASE("Camera inverse projection")
{
    vp::rendering::Camera c;
    vp::AffineTransform t = vp::AffineTransform::Identity();
    t.translate(vp::Vector(10, 10, 10));
    
    c.setCameraToImage(vp::Vector2(100, 100), vp::Vector2(320, 240));
    c.setCameraToWorld(t);
    
    vp::rendering::Camera::Matrix34 c2w = c.cameraToWorld();
    vp::Vector x = c2w * vp::Vector(-10,-10,40).homogeneous();
    
    REQUIRE_CLOSE(x(0), 0);
    REQUIRE_CLOSE(x(1), 0);
    REQUIRE_CLOSE(x(2), 50);
    
    vp::rendering::Camera::Matrix33 i2c = c.imageToCamera();
    
    x = i2c * vp::Vector2((100 * -10 + 320 * 40) / vp::Scalar(40),(100 * -10 + 240 * 40) / vp::Scalar(40)).homogeneous();
    x *= 40;
    
    REQUIRE_CLOSE(x(0), -10);
    REQUIRE_CLOSE(x(1), -10);
    REQUIRE_CLOSE(x(2), 40);
}

TEST_CASE("Ray generation")
{
    vp::rendering::Camera c;
    vp::AffineTransform t = vp::AffineTransform::Identity();
    t.rotate(Eigen::AngleAxis<vp::Scalar>(0.5 * M_PI, vp::Vector::UnitX()));
    
    c.setCameraToImage(vp::Vector2(100, 100), vp::Vector2(320, 240));
    c.setCameraToWorld(t);
    
    vp::Vector r;
    c.generateCameraRay(vp::Vector2(320,240), r);
    
    REQUIRE_CLOSE(r.norm(), 1);
    REQUIRE_CLOSE(r(0), 0);
    REQUIRE_CLOSE(r(1), 0);
    REQUIRE_CLOSE(r(2), 1);
    
    vp::AffineTransform c2w = c.cameraToWorldTransform();
    r = c2w.linear() * r;

    REQUIRE_CLOSE(r.norm(), 1);
    REQUIRE_CLOSE(r(0), 0);
    REQUIRE_CLOSE(r(1), -1);
    REQUIRE_CLOSE(r(2), 0);
    
    std::vector<vp::Vector> dirs;
    c.generateCameraRays(6, 12, dirs);
    
    c.generateCameraRay(vp::Vector2(0,0), r);
    REQUIRE(dirs.size() == 12 * 6);
    REQUIRE_CLOSE(dirs[0](0), r(0));
    REQUIRE_CLOSE(dirs[0](1), r(1));
    REQUIRE_CLOSE(dirs[0](2), r(2));
    
    c.generateCameraRay(vp::Vector2(5,0), r);
    REQUIRE_CLOSE(dirs[5](0), r(0));
    REQUIRE_CLOSE(dirs[5](1), r(1));
    REQUIRE_CLOSE(dirs[5](2), r(2));
    
    c.generateCameraRay(vp::Vector2(0,1), r);
    REQUIRE_CLOSE(dirs[12](0), r(0));
    REQUIRE_CLOSE(dirs[12](1), r(1));
    REQUIRE_CLOSE(dirs[12](2), r(2));
}

