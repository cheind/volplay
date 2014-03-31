// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/rendering/depth_image_generator.h>
#include <volplay/rendering/renderer.h>
#include <volplay/rendering/image.h>
#include <volplay/rendering/camera.h>
#include <iostream>

namespace volplay {
    
    namespace rendering {
    
        DepthImageGenerator::DepthImageGenerator()
        :_invValue(0.f), _image(new FloatImage())
        {}
        
        void
        DepthImageGenerator::onRenderingBegin(const Renderer *r)
        {
            _to = r->primaryTraceOptions();
            _worldToCamera = r->camera()->worldToCameraTransform();
            _image->create(r->imageHeight(), r->imageWidth(), 1);
        }
        
        void
        DepthImageGenerator::onRowBegin(int row)
        {            
            _row = _image->row(row);
        }
        
        void
        DepthImageGenerator::onUpdatePixel(const PixelInfo &pi)
        {
            if (pi.tr.hit) {
                _row[pi.col] = (_worldToCamera * (pi.origin + pi.tr.t * pi.direction)).z();
            } else {
                _row[pi.col] = _invValue;
            }            
        }
        
        void
        DepthImageGenerator::onRenderingComplete(const Renderer *r)
        {
            // Nothing todo            
        }
        
        FloatImagePtr
        DepthImageGenerator::image() const
        {
            return _image;
        }

        void
        DepthImageGenerator::setInvalidDepthValue(float v)
        {
            _invValue = v;
        }
        
    }
}