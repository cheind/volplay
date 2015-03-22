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
#include <volplay/rendering/image_generator.h>
#include <volplay/sdf_node.h>

namespace volplay {
    
    namespace rendering {
    
        Renderer::Renderer()
        : _imageWidth(0), _imageHeight(0)
        {}
        
        void
        Renderer::setCamera(const CameraPtr &cam)
        {
            _camera = cam;
        }

        const CameraPtr &
        Renderer::camera() const
        {
            return _camera;
        }
        
        void
        Renderer::setScene(const SDFNodePtr &root)
        {
            _root = root;
        }
        
        const SDFNodePtr &
        Renderer::scene() const
        {
            return _root;
        }
        
        void
        Renderer::setImageResolution(int imageHeight, int imageWidth)
        {
            _imageHeight = imageHeight;
            _imageWidth = imageWidth;
        }
        
        int
        Renderer::imageHeight() const
        {
            return _imageHeight;
        }
        
        int
        Renderer::imageWidth() const
        {
            return _imageWidth;
        }
        
        void
        Renderer::setPrimaryTraceOptions(const SDFNode::TraceOptions &opts)
        {
            _primaryTraceOptions = opts;
        }
        
        const SDFNode::TraceOptions &
        Renderer::primaryTraceOptions() const
        {
            return _primaryTraceOptions;
        }
        
        void
        Renderer::addImageGenerator(const ImageGeneratorPtr &g)
        {
            _generators.push_back(g);
        }
        
        void
        Renderer::setLights(const std::vector<LightPtr> &l)
        {
            _lights = l;
        }
        
        const std::vector<LightPtr> &
        Renderer::lights() const
        {
            return _lights;
        }
        
        void
        Renderer::render()
        {
            if (!_camera || !_root || _imageWidth == 0|| _imageHeight == 0) {
                return;
            }
            
            // Prepare primary rays
            std::vector<Vector, Eigen::aligned_allocator<Vector> > rays;
            _camera->generateCameraRays(_imageHeight, _imageWidth, rays);
           
            std::vector<SDFNode::TraceResult> traceResults(rays.size());
            AffineTransform::LinearPart t = _camera->cameraToWorldTransform().linear();
            
            // Convert to world rays and trace
            const Vector origin = _camera->originInWorld();
            for (size_t i = 0; i < rays.size(); ++i) {
                rays[i] = Vector(t * rays[i]); // Note: explict Vector() needed here since introduction of aligned allocators.
                _root->trace(origin, rays[i], _primaryTraceOptions, &traceResults[i]);
            }
            
            // Prepare generators
            std::vector<ImageGeneratorPtr>::iterator gBegin = _generators.begin();
            std::vector<ImageGeneratorPtr>::iterator gEnd = _generators.end();
            std::vector<ImageGeneratorPtr>::iterator gi;
            
            for (gi = gBegin; gi != gEnd; ++gi) {
                (*gi)->onRenderingBegin(this);
            }

            // For each row invoke generators
            for (int row = 0; row < _imageHeight; ++row) {
                
                const Vector *rayRow = &rays[row * _imageWidth];
                const SDFNode::TraceResult *traceResultsRow = &traceResults[row * _imageWidth];
                
                for (gi = gBegin; gi != gEnd; ++gi) {
                    (*gi)->onUpdateRow(row, origin, rayRow, traceResultsRow, _imageWidth);
                }
            }
            
            // Finish generators
            for (gi = gBegin; gi != gEnd; ++gi) {
                (*gi)->onRenderingComplete(this);
            }
        }
        
        
    }
}