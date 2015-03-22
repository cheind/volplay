// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_DEPTH_IMAGE_GENERATOR
#define VOLPLAY_DEPTH_IMAGE_GENERATOR

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <volplay/rendering/image_generator.h>

namespace volplay {
    
    namespace rendering {

        /** Generates a depth image. 
         *
         *  The depth image stores the parallel distance between the camera's image plane and
         *  the ray intersection point for each pixel.
         *
         *  The depth image is stored with floating point accuracy. A special constant value
         *  for invalid depth values can be set by the user.
         */
        class DepthImageGenerator : public ImageGenerator {
        public:
            /** Default constructor */
            DepthImageGenerator();
            
            virtual void onRenderingBegin(const Renderer *r);
            virtual void onUpdateRow(int row,
                                     const Vector &origin,
                                     const Vector *directions,
                                     const SDFNode::TraceResult *tr, int cols);
            virtual void onRenderingComplete(const Renderer *r);
            
            /** Access the generated heat image. */
            ScalarImagePtr image() const;

            /** Set the floating point value representing invalid depth measurements. 
             *  By default this is zero. */
            void setInvalidDepthValue(Scalar value);
            
        private:
            Scalar _invValue;
            ScalarImagePtr _image;
            SDFNode::TraceOptions _to;
            AffineTransform _worldToCamera;
        };
        
    }
}

#endif
