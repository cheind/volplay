// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_BLINN_PHONG_IMAGE_GENERATOR
#define VOLPLAY_BLINN_PHONG_IMAGE_GENERATOR

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <volplay/rendering/image_generator.h>

namespace volplay {
    
    namespace rendering {

        /** Generates a color image using the Blinn-Phong reflection model.
         *
         *  SDFNodes need to carry a 'Material' attachment of type Material.
         */
        class BlinnPhongImageGenerator : public ImageGenerator {
        public:
            /** Default constructor */
            BlinnPhongImageGenerator();
            
            virtual void onRenderingBegin(const Renderer *r);
            virtual void onUpdateRow(int row,
                                     const Vector &origin,
                                     const Vector *directions,
                                     const SDFNode::TraceResult *tr, int cols);
            virtual void onRenderingComplete(const Renderer *r);
            
            /** Access the generated BGR color image. */
            ByteImagePtr image() const;
            
        private:
            
            /** Illuminate point. */
            Vector illuminate(const Vector &viewDir, const Vector &p) const;
            
            ByteImagePtr _image;
            SDFNodePtr _root;
            LightPtr _light;
            Vector _clearColor;
                                    
        };
        
    }
}

#endif
