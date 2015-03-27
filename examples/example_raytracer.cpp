// This file is part of volplay, a library for interacting with volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include "catch.hpp"
#include <volplay/volplay.h>
#include <opencv2/opencv.hpp>

namespace vp = volplay;
namespace vpr = volplay::rendering;

cv::Mat toBgr(const cv::Mat &rgb) {
    cv::Mat bgr;
#if CV_VERSION_MAJOR >= 3
    cv::cvtColor(rgb, bgr, cv::COLOR_RGB2BGR);
#else
	cv::cvtColor(rgb, bgr, CV_RGB2BGR);
#endif
    return bgr;
}

TEST_CASE("CPU based raytracing")
{
    const vp::Scalar aspect = vp::Scalar(16)/9;
    const int imageWidth = 720;
    const int imageHeight = (int)(vp::Scalar(imageWidth) / aspect);
    
    vpr::MaterialPtr m1(new vpr::Material());
    m1->setDiffuseColor(vp::Vector(vp::S(0.1), vp::S(0.6), vp::S(0.1)));
    m1->setAmbientColor(m1->diffuseColor() * vp::S(0.1));
    m1->setSpecularColor(vp::Vector::Ones());
    m1->setSpecularHardness(vp::S(128));
    
    vpr::MaterialPtr m2(new vpr::Material());
    m2->setDiffuseColor(vp::Vector(vp::S(0.6), vp::S(0.1), vp::S(0.1)));
    m2->setAmbientColor(m2->diffuseColor() * vp::S(0.1));
    m2->setSpecularColor(vp::Vector::Ones());
    m2->setSpecularHardness(vp::S(128));
    
    vp::SDFNodePtr scene = vp::make()
        .join()
            .plane().normal(vp::Vector::UnitY())
            .transform().translate(vp::Vector(0, 1, 0))
                .sphere().radius(1).attach("Material", m1)
            .end()
            .transform().translate(vp::Vector(3, 1, 0))
                .sphere().radius(1).attach("Material", m2)
            .end()
        .end();
    
    vpr::CameraPtr cam(new vpr::Camera());
    cam->setCameraToImage(imageHeight, imageWidth, vp::Scalar(0.40));
    cam->setCameraToWorldAsLookAt(vp::Vector(-5,5,10), vp::Vector(0,0,0), vp::Vector(0,1,0));
    
    std::vector<vpr::LightPtr> lights;
    lights.push_back(vpr::Light::createPointLight(vp::Vector(20,15,20), vp::Vector::Ones(), vp::Vector::Ones(), vp::Vector::Ones(), 50));
    lights.push_back(vpr::Light::createPointLight(vp::Vector(-20,15,-20), vp::Vector::Ones(), vp::Vector::Ones(), vp::Vector::Ones(), 50));
    
    vpr::RendererPtr r(new vpr::Renderer());
    r->setScene(scene);
    r->setCamera(cam);
    r->setLights(lights);
    r->setImageResolution(imageHeight, imageWidth);
    
    vp::SDFNode::TraceOptions to;
    to.maxIter = 1000;
    to.stepFact = vp::Scalar(0.9);
    r->setPrimaryTraceOptions(to);
    
    vpr::HeatImageGeneratorPtr heat(new vpr::HeatImageGenerator());
    r->addImageGenerator(heat);

    vpr::DepthImageGeneratorPtr depth(new vpr::DepthImageGenerator());
    r->addImageGenerator(depth);

    vpr::BlinnPhongImageGeneratorPtr phong(new vpr::BlinnPhongImageGenerator());
    phong->setAntialiasingEnabled(true);
    r->addImageGenerator(phong);
    
    r->render();

    cv::Mat depthImage, depthImageFloat;
    depthImageFloat = depth->image()->toOpenCV();
    double minDepth, maxDepth;
    cv::minMaxLoc(depthImageFloat, &minDepth, &maxDepth);
    depthImageFloat.convertTo(depthImage, CV_8UC1, 255 / maxDepth);
    
    cv::imshow("Heat Image", heat->image()->toOpenCV());
    cv::imshow("Depth Image",depthImage);
    cv::imshow("Blinn Phong Image",toBgr(phong->image()->toOpenCV()));
    
    
    cv::waitKey();

}