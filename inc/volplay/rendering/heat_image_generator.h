// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_HEAT_IMAGE_GENERATOR
#define VOLPLAY_HEAT_IMAGE_GENERATOR

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <volplay/rendering/image_generator.h>

namespace volplay {
    
    namespace rendering {

        /** Generates a heat image. 
         *
         *  The heat image shows for each pixel how many ray steps where necessary
         *  to produce a result. The more steps required the higher the image intensity.
         *  In general along object edges sphere tracing will take more steps.
         */
        class HeatImageGenerator : public ImageGenerator {
        public:
            /** Default constructor */
            HeatImageGenerator();
            
            virtual void onRenderingBegin(const Renderer *r);
            virtual void onUpdateRow(int row,
                                     const Vector &origin,
                                     const Vector *directions,
                                     const SDFNode::TraceResult *tr, int cols);
            virtual void onRenderingComplete(const Renderer *r);
            
            /** Access the generated heat image. */
            ByteImagePtr image() const;
            
        private:
            int _maxIter;
            ByteImagePtr _image;
        };
        
    }
}

#endif
