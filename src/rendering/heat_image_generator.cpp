// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/rendering/heat_image_generator.h>
#include <volplay/rendering/renderer.h>
#include <volplay/rendering/image.h>
#include <volplay/rendering/saturate.h>

namespace volplay {
    
    namespace rendering {
    
        HeatImageGenerator::HeatImageGenerator()
        :_maxIter(500), _image(new ByteImage())
        {}
        
        void
        HeatImageGenerator::onRenderingBegin(const Renderer *r)
        {
            _maxIter = r->primaryTraceOptions().maxIter;
            _image->create(r->imageHeight(), r->imageWidth(), 1);
        }
        
        
        void
        HeatImageGenerator::onUpdateRow(int row,
                                        const Vector &origin,
                                        const Vector *directions,
                                        const SDFNode::TraceResult *tr, int cols)
        {
            unsigned char *imageRow = _image->row(row);
            
            for (int c =0; c < cols; ++c) {
                imageRow[c] = saturate<unsigned char>(((Scalar)tr[c].iter / _maxIter) * Scalar(255));
            }
        }
        
        void
        HeatImageGenerator::onRenderingComplete(const Renderer *r)
        {
            // Nothing todo
        }
        
        ByteImagePtr
        HeatImageGenerator::image() const
        {
            return _image;
        }
        
    }
}