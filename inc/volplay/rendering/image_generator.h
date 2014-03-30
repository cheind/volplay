// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_IMAGE_GENERATOR
#define VOLPLAY_IMAGE_GENERATOR

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <volplay/sdf_node.h>
#include <volplay/rendering/image_generator.h>

namespace volplay {
    
    namespace rendering {

        /** Abstract base class for generating one or more rendered images. */
        class ImageGenerator {
        public:
            
            /** Info about the current pixel to be traced. */
            struct PixelInfo {
                int row;
                int col;
                Vector origin;
                Vector direction;
                SDFNode::TraceResult tr;
            };
            
            virtual void onRenderingBegin(const Renderer *r) = 0;
            virtual void onRowBegin(int row) = 0;
            virtual void onUpdatePixel(const PixelInfo &pi) = 0;
            virtual void onRenderingComplete(const Renderer *r) = 0;
        };
        
    }
}

#endif
