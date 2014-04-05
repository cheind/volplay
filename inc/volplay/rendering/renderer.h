// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_RENDERER
#define VOLPLAY_RENDERER

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <volplay/sdf_node.h>
#include <volplay/rendering/image.h>
#include <vector>

namespace volplay {
    
    namespace rendering {

        /** Renders SDF scenes to images solely on CPU resources. */
        class Renderer {
        public:
            
            /** Default initializer. */
            Renderer();
            
            /** Set the SDF scene to be rendered. */
            void setScene(const SDFNodePtr &root);
            
            /** Acces the SDF scene to be rendered. */
            const SDFNodePtr &scene() const;
            
            /** Set the camera object */
            void setCamera(const CameraPtr &cam);

            /** Access the camera */
            const CameraPtr &camera() const;
            
            /** Set array of lights */
            void setLights(const std::vector<LightPtr> &l);
            
            /** Access the i-th light source. */
            const std::vector<LightPtr> &lights() const;
            
            /** Set the resulting image resolution. */
            void setImageResolution(int imageHeight, int imageWidth);
            
            /** Access image height. */
            int imageHeight() const;
            
            /** Access image width. */
            int imageWidth() const;
            
            /** Set the default trace options for primary rays. */
            void setPrimaryTraceOptions(const SDFNode::TraceOptions &opts);
            
            /** Access the primary trace options. */
            const SDFNode::TraceOptions &primaryTraceOptions() const;
            
            /** Add a new image generator */
            void addImageGenerator(const ImageGeneratorPtr &g);
            
            /** Render the scene */
            void render();
            
            
        private:
            SDFNodePtr _root;
            CameraPtr _camera;
            int _imageWidth, _imageHeight;
            SDFNode::TraceOptions _primaryTraceOptions;
            
            std::vector<ImageGeneratorPtr> _generators;
            std::vector<LightPtr> _lights;
        };
        
    }
}

#endif
