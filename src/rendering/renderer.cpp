// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/rendering/camera.h>
#include <volplay/rendering/renderer.h>
#include <volplay/rendering/saturate.h>
#include <volplay/sdf_node.h>

namespace volplay {
    
    namespace rendering {
    
        Renderer::Renderer()
        : _imageWidth(0), _imageHeight(0), _heatImage(new Image<unsigned char>())
        {}
        
        void
        Renderer::setCamera(const CameraPtr &cam)
        {
            _camera = cam;
        }
        
        void
        Renderer::setScene(const SDFNodePtr &root)
        {
            _root = root;
        }
        
        void
        Renderer::setImageResolution(int imageWidth, int imageHeight)
        {
            _imageHeight = imageHeight;
            _imageWidth = imageWidth;
        }
        
        void
        Renderer::setPrimaryTraceOptions(const SDFNode::TraceOptions &opts)
        {
            _primaryTraceOptions = opts;
        }
        
        
        Renderer::ByteImagePtr
        Renderer::heatImage() const
        {
            return _heatImage;
        }
        
        void
        Renderer::render()
        {
            if (!_camera || !_root || _imageWidth == 0|| _imageHeight == 0) {
                return;
            }
            
            // Allocate images.
            _heatImage->create(_imageHeight, _imageWidth, 1);
            
            // Prepare rays
            std::vector<Vector> rays;
            _camera->generateCameraRays(_imageHeight, _imageWidth, rays);
            Vector origin = _camera->originInWorld();
            AffineTransform::LinearPart t = _camera->cameraToWorldTransform().linear();
            
            // Trace images
            SDFNode::TraceResult tr;
            
            for (int r = 0; r < _imageHeight; ++r) {
                const Vector *rayRow = &rays[r * _imageWidth];
                unsigned char *heatRow = _heatImage->row(r);

                for (int c = 0; c < _imageWidth; ++c) {
                    
                    // Primary ray
                    _root->trace(origin, t * rayRow[c], _primaryTraceOptions, &tr);

                    heatRow[c] = saturate<unsigned char>(((float)tr.iter / _primaryTraceOptions.maxIter) * 255.f);
                }
            }  
        }
        
        
    }
}