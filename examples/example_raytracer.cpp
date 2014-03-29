// This file is part of volplay, a library for interacting with volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include "catch.hpp"
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_repetition.h>
#include <volplay/sdf_making.h>
#include <volplay/sdf_rigid_transform.h>
#include <volplay/camera.h>

#include <opencv2/opencv.hpp>

namespace vp = volplay;

TEST_CASE("Raytracing simple scene using phong shading and a single light source")
{
    vp::SDFRepetitionPtr nrep = vp::makeSDFRepetition(vp::Vector(150, 150, 150), vp::makeSDFSphere(10));
    
    vp::AffineTransform at = vp::AffineTransform::Identity();
    at.translate(vp::Vector(0,0,100));
    vp::SDFNodePtr n = vp::SDFRigidTransformPtr(new vp::SDFRigidTransform(at, nrep));;
    
    const int imageWidth = 640;
    const int imageHeight = 480;
    
    vp::Camera cam;
    cam.setCameraToImage(imageWidth, imageHeight, vp::Scalar(0.99), vp::Scalar(0.75));
    cam.setCameraToImage(imageWidth, imageHeight, vp::Scalar(0.40));
    
    std::vector<vp::Vector> rays;
    cam.generateCameraRays(imageWidth, imageHeight, rays);
    
    vp::SDFNode::TraceOptions to;
    
    cv::Mat img(imageHeight, imageWidth, CV_8UC1);
    for (int r = 0; r < img.rows; ++r) {
        for (int c = 0; c < img.cols; ++c) {
            vp::Scalar s = n->trace(cam.originInWorld(), rays[r * imageWidth + c], to);
            if (s < 1000) {
                img.at<unsigned char>(r, c) = s * 255 / 1000;
            } else {
                img.at<unsigned char>(r, c) = 0;
            }
        }
    }
    
    cv::imshow("image", img);
    cv::waitKey();

}