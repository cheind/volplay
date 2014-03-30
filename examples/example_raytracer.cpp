// This file is part of volplay, a library for interacting with volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include "catch.hpp"
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_make.h>
#include <volplay/sdf_rigid_transform.h>
#include <volplay/rendering/camera.h>

#include <opencv2/opencv.hpp>

namespace vp = volplay;

TEST_CASE("CPU based raytracing")
{
    const int imageWidth = 640;
    const int imageHeight = 480;
    
    vp::SDFNodePtr scene = vp::SDFMake::plane().join(vp::SDFMake::sphere(1).translate(vp::Vector(0,0,0)));
    
    vp::rendering::Camera cam;
    cam.setCameraToImage(imageWidth, imageHeight, vp::Scalar(0.40));
    cam.setCameraToWorldAsLookAt(vp::Vector(0,0,5), vp::Vector(0,0,0), vp::Vector(0,-1,0));
    
    vp::AffineTransform t = cam.cameraToWorldTransform();
    
    std::vector<vp::Vector> rays;
    cam.generateCameraRays(imageWidth, imageHeight, rays);
    
    vp::SDFNode::TraceOptions to;
    
    cv::Mat img(imageHeight, imageWidth, CV_8UC1);
    for (int r = 0; r < img.rows; ++r) {
        for (int c = 0; c < img.cols; ++c) {
            vp::Scalar s = scene->trace(cam.originInWorld(), t.linear() * rays[r * imageWidth + c], to);
            if (s < 10) {
                img.at<unsigned char>(r, c) = s * 255 / 500;
            } else {
                img.at<unsigned char>(r, c) = 0;
            }
        }
    }
    
    cv::imshow("image", img);
    cv::waitKey();

}