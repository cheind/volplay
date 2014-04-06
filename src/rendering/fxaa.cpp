// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/rendering/fxaa.h>
#include <volplay/rendering/image.h>
#include <volplay/rendering/saturate.h>

namespace volplay {
    
    namespace rendering {
    
        FXAA::FXAA()
        :_image(new FloatImage())
        {
        }
        
        /** Access a pixel in the image. Repeat border if outside. Returned pixel is normalized. */
        inline Vector pixel(int row, int col, const FloatImagePtr &img) {
            int rows = img->rows();
            int cols = img->cols();
            
            // Repeat border if outside
            row = clamp(row, 0, rows - 1);
            col = clamp(col, 0, cols - 1);
            
            float *pixelRow = img->row(row);
            return Vector(pixelRow[col*3+0],
                          pixelRow[col*3+1],
                          pixelRow[col*3+2]);
        }
        
        /** Same as above but for floating point values. */
        inline Vector pixel(Scalar row, Scalar col, const ByteImagePtr &img) {
            return pixel((int)round(row), (int)round(col), img);
        }
        
        FloatImagePtr
        FXAA::filter(const FloatImagePtr &src)
        {
            _image->create(src->rows(), src->cols(), src->channels());
            
            for (int r = 0; r < src->rows(); ++r) {
                float *imgRow = _image->row(r);
                
                for (int c = 0; c < src->cols(); ++c) {
                    Eigen::Map<Vector> out = imgRow + c*3;
                    Vector p = filterPixel(r, c, src);
                    out = p;
                }
            }
            
            return _image;
        }
        
// Shortcut for minimum
#define SMIN(a, b) std::min<Scalar> ((a), (b))
#define SMAX(a, b) std::max<Scalar> ((a), (b))
        
        Vector
        FXAA::filterPixel(int row, int col, const FloatImagePtr &img) const
        {
            const Vector rgbNW = pixel(row-1, col-1, img);
            const Vector rgbNE = pixel(row-1, col+1, img);
            const Vector rgbSW = pixel(row+1, col-1, img);
            const Vector rgbSE = pixel(row+1, col+1, img);
            const Vector rgbM =  pixel(row, col, img);
            
            const Vector luma = Vector(0.299, 0.587, 0.114);
            float lumaNW = rgbNW.dot(luma);
            float lumaNE = rgbNE.dot(luma);
            float lumaSW = rgbSW.dot(luma);
            float lumaSE = rgbSE.dot(luma);
            float lumaM = rgbM.dot(luma);
            
            float lumaMin = SMIN(lumaNW, SMIN(lumaNE, SMIN(lumaSW, SMIN(lumaSE, lumaM))));
            float lumaMax = SMAX(lumaNW, SMAX(lumaNE, SMAX(lumaSW, SMAX(lumaSE, lumaM))));
            
            // FXAA params - fixed here for now.
            const Scalar fxaa_reduce_mul = Scalar(1) / Scalar(32);
            const Scalar fxaa_reduce_min = Scalar(1) / Scalar(128);
            const Scalar fxaa_span_max = Scalar(8);
            
            // Find direction and blur orthogonal to edge.
            
            Vector2 dir;
            dir.x() = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
            dir.y() = ((lumaNW + lumaSW) - (lumaNE + lumaSE));
            
            float dirRed = SMAX((lumaNW + lumaNE + lumaSW + lumaSE) * (Scalar(0.25) * fxaa_reduce_mul), fxaa_reduce_min);
            float invDirMin = Scalar(1) / (SMIN(abs(dir.x()), abs(dir.y())) + dirRed);
            
            dir *= invDirMin;
            dir.x() = clamp(dir.x(), -fxaa_span_max, fxaa_span_max);
            dir.y() = clamp(dir.y(), -fxaa_span_max, fxaa_span_max);
            
            const Scalar c = Scalar(1)/Scalar(3) - Scalar(0.5);
            const Scalar d = Scalar(2)/Scalar(3) - Scalar(0.5);
            const Scalar e = Scalar(0)/Scalar(3) - Scalar(0.5);
            const Scalar f = Scalar(3)/Scalar(3) - Scalar(0.5);
                              
            Vector rgbA = Scalar(0.5) * (pixel(row + c, col + c, img) + pixel(row + d, col + d, img));
            Vector rgbB = rgbA * Scalar(0.5) + Scalar(0.25) * (pixel(row + e, col + e, img) + pixel(row + f, col + f, img));
            float lumaB = rgbB.dot(luma);
            
            if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
                return rgbA;
            } else {
                return rgbB;
            }
        }
    }
}