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
#include <volplay/rendering/camera.h>
#include <volplay/rendering/image.h>
#include <volplay/rendering/renderer.h>
#include <volplay/rendering/heat_image_generator.h>
#include <volplay/rendering/depth_image_generator.h>
#include <volplay/rendering/blinn_phong_image_generator.h>

#include <opencv2/opencv.hpp>

namespace vp = volplay;

TEST_CASE("CPU based raytracing")
{
    const int imageWidth = 640;
    const int imageHeight = 480;
    
    vp::SDFNodePtr scene = vp::SDFMake::plane().join(vp::SDFMake::sphere(1).translate(vp::Vector(0,0,0)));
    
    vp::rendering::CameraPtr cam(new vp::rendering::Camera());
    cam->setCameraToImage(imageHeight, imageWidth, vp::Scalar(0.40));
    cam->setCameraToWorldAsLookAt(vp::Vector(0,0,5), vp::Vector(0,0,0), vp::Vector(0,-1,0));
    
    vp::rendering::RendererPtr r(new vp::rendering::Renderer());
    r->setScene(scene);
    r->setCamera(cam);
    r->setImageResolution(imageHeight, imageWidth);
    
    vp::SDFNode::TraceOptions to;
    to.maxIter = 200;
    r->setPrimaryTraceOptions(to);
    
    vp::rendering::HeatImageGeneratorPtr heat(new vp::rendering::HeatImageGenerator());
    r->addImageGenerator(heat);

    vp::rendering::DepthImageGeneratorPtr depth(new vp::rendering::DepthImageGenerator());
    r->addImageGenerator(depth);

    vp::rendering::BlinnPhongImageGeneratorPtr phong(new vp::rendering::BlinnPhongImageGenerator());
    r->addImageGenerator(phong);
    
    r->render();

    cv::Mat depthImage;
    depth->image()->toOpenCV().convertTo(depthImage, CV_8UC1, 20);
    
    cv::imshow("Heat Image", heat->image()->toOpenCV());
    cv::imshow("Depth Image",depthImage);
    cv::imshow("Blinn Phong Image",phong->image()->toOpenCV());
    cv::waitKey();

}