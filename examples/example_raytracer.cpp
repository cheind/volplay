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
#include <volplay/rendering/fxaa.h>
#include <volplay/rendering/material.h>
#include <volplay/rendering/light.h>

#include <opencv2/opencv.hpp>

namespace vp = volplay;
namespace vpr = volplay::rendering;

cv::Mat toBgr(const cv::Mat &rgb) {
    cv::Mat bgr;
    cv::cvtColor(rgb, bgr, CV_RGB2BGR);
    return bgr;
}

TEST_CASE("CPU based raytracing")
{
    const int imageWidth = 640;
    const int imageHeight = 480;
    
    vpr::MaterialPtr m1(new vpr::Material());
    vpr::MaterialPtr m2(new vpr::Material());
    vpr::MaterialPtr m3(new vpr::Material());
    
    m1->setDiffuseColor(vp::Vector(1, 0, 0));
    m2->setDiffuseColor(vp::Vector(0, 1, 0));
    m3->setDiffuseColor(vp::Vector(0, 0, 1));
    
    m1->setSpecularColor(vp::Vector::Zero());
    m2->setSpecularColor(vp::Vector::Zero());
    m3->setSpecularColor(vp::Vector::Zero());
    
    vp::SDFNodePtr scene =
        vp::SDFMake::plane(vp::Vector::UnitX()).attach("Material", m1) +
        vp::SDFMake::plane(vp::Vector::UnitY()).attach("Material", m2) +
        vp::SDFMake::plane(vp::Vector::UnitZ()).attach("Material", m3) +
        vp::SDFMake::sphere(1).translate(vp::Vector(1, 1, 1));
    
    vpr::CameraPtr cam(new vpr::Camera());
    cam->setCameraToImage(imageHeight, imageWidth, vp::Scalar(0.40));
    cam->setCameraToWorldAsLookAt(vp::Vector(10,10,10), vp::Vector(0,0,0), vp::Vector(0,1,0));
    
    std::vector<vpr::LightPtr> lights;
    lights.push_back(vpr::Light::createPointLight(vp::Vector(20,20,20), vp::Vector::Zero(), vp::Vector::Ones(), vp::Vector::Ones(), 100));
    lights.push_back(vpr::Light::createPointLight(vp::Vector(0,3,0), vp::Vector::Zero(), vp::Vector(0,1,0), vp::Vector(0,1,0), 2));
    
    vpr::RendererPtr r(new vpr::Renderer());
    r->setScene(scene);
    r->setCamera(cam);
    r->setLights(lights);
    r->setImageResolution(imageHeight, imageWidth);
    
    vp::SDFNode::TraceOptions to;
    to.maxIter = 500;
    r->setPrimaryTraceOptions(to);
    
    vpr::HeatImageGeneratorPtr heat(new vpr::HeatImageGenerator());
    r->addImageGenerator(heat);

    vpr::DepthImageGeneratorPtr depth(new vpr::DepthImageGenerator());
    r->addImageGenerator(depth);

    vpr::BlinnPhongImageGeneratorPtr phong(new vpr::BlinnPhongImageGenerator());
    r->addImageGenerator(phong);
    
    r->render();
    
    vpr::FXAAPtr fxaa(new vpr::FXAA());

    cv::Mat depthImage;
    depth->image()->toOpenCV().convertTo(depthImage, CV_8UC1, 255. / 10.);
    
    cv::imshow("Heat Image", heat->image()->toOpenCV());
    cv::imshow("Depth Image",depthImage);
    cv::imshow("Blinn Phong Image",toBgr(phong->image()->toOpenCV()));
    cv::imshow("Blinn Phong Image FXAA",toBgr(fxaa->filter(phong->image())->toOpenCV()));
    cv::waitKey();

}