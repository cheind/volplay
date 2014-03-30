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

namespace volplay {
    
    namespace rendering {

        /** Renders SDF scenes to images solely on CPU resources. */
        class Renderer {
        public:
            /** Image type with byte sized channels. */
            typedef std::shared_ptr< Image<unsigned char> > ByteImagePtr;
            
            /** Default initializer. */
            Renderer();
            
            /** Set the SDF scene to be rendered. */
            void setScene(const SDFNodePtr &root);
            
            /** Set the camera object */
            void setCamera(const CameraPtr &cam);
            
            /** Set the resulting image resolution. */
            void setImageResolution(int imageWidth, int imageHeight);
            
            /** Set the default trace options for primary rays. */
            void setPrimaryTraceOptions(const SDFNode::TraceOptions &opts);
            
            /** Render the scene */
            void render();
            
            /** Access the heat image.
             *  The heat image shows for each pixel how many ray steps where necessary
             *  to produce a result. The more steps required the higher the image intensity. 
             *  In general along object edges sphere tracing will take more steps.
             */
            ByteImagePtr heatImage() const;
            
        private:
            SDFNodePtr _root;
            CameraPtr _camera;
            int _imageWidth, _imageHeight;
            SDFNode::TraceOptions _primaryTraceOptions;
            
            ByteImagePtr _heatImage;
        };
        
    }
}

#endif
