// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/rendering/blinn_phong_image_generator.h>
#include <volplay/rendering/renderer.h>
#include <volplay/rendering/image.h>
#include <volplay/rendering/saturate.h>

namespace volplay {
    
    namespace rendering {
    
        BlinnPhongImageGenerator::BlinnPhongImageGenerator()
        :_image(new ByteImage())
        {}
        
        void
        BlinnPhongImageGenerator::onRenderingBegin(const Renderer *r)
        {
            _image->create(r->imageHeight(), r->imageWidth(), 1);
        }
        
        
        void
        BlinnPhongImageGenerator::onUpdateRow(int row,
                                        const Vector &origin,
                                        const Vector *directions,
                                        const SDFNode::TraceResult *tr, int cols)
        {
        }
        
        void
        BlinnPhongImageGenerator::onRenderingComplete(const Renderer *r)
        {
            // Nothing todo
        }
        
        ByteImagePtr
        BlinnPhongImageGenerator::image() const
        {
            return _image;
        }
        
    }
}