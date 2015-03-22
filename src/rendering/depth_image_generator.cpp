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
        :_invValue(Scalar(0)), _image(new ScalarImage())
        {}
        
        void
        DepthImageGenerator::onRenderingBegin(const Renderer *r)
        {
            _to = r->primaryTraceOptions();
            _worldToCamera = r->camera()->worldToCameraTransform();
            _image->create(r->imageHeight(), r->imageWidth(), 1);
        }
        
        void
        DepthImageGenerator::onUpdateRow(int row,
                                         const Vector &origin,
                                         const Vector *directions,
                                         const SDFNode::TraceResult *tr, int cols)
        {
            Scalar *imageRow = _image->row(row);
            
            for (int c = 0; c < cols; ++c) {
                if (tr[c].hit) {
                    // Note that ray is traced in world coordinate sytem.
                    imageRow[c] = (_worldToCamera * (origin + tr[c].t * directions[c])).z();
                } else {
                    imageRow[c] = _invValue;
                }
            }
        }
        
        void
        DepthImageGenerator::onRenderingComplete(const Renderer *r)
        {
            // Nothing todo            
        }
        
        ScalarImagePtr
        DepthImageGenerator::image() const
        {
            return _image;
        }

        void
        DepthImageGenerator::setInvalidDepthValue(Scalar v)
        {
            _invValue = v;
        }
        
    }
}