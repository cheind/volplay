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
        :_image(new ScalarImage())
        {
        }
        
        inline Vector bilinearSample(Scalar row, Scalar col, const ScalarImagePtr &img) {
            int x = static_cast<int>(col);
            int y = static_cast<int>(row);
            
            Scalar x_ratio = col - x;
            Scalar y_ratio = row - y;
            Scalar x_opp = Scalar(1) - x_ratio;
            Scalar y_opp = Scalar(1) - y_ratio;
            
            typedef Eigen::Map<Vector> MVector;
            MVector a = img->elementClamped(y, x);
            MVector b = img->elementClamped(y, x+1);
            MVector c = img->elementClamped(y+1, x);
            MVector d = img->elementClamped(y+1, x+1);
            
            return (a * x_opp + b * x_ratio) * y_opp + (c * x_opp + d * x_ratio) * y_ratio;
        }
        
        /** Access a pixel in the image. Repeat border if outside. Returned pixel is normalized. */
        inline Vector pixel(int row, int col, const ScalarImagePtr &img) {
            int rows = img->rows();
            int cols = img->cols();
            
            // Repeat border if outside
            row = clamp(row, 0, rows - 1);
            col = clamp(col, 0, cols - 1);
            
            Scalar *pixelRow = img->row(row);
            return Vector(pixelRow[col*3+0],
                          pixelRow[col*3+1],
                          pixelRow[col*3+2]);
        }
        
        ScalarImagePtr
        FXAA::filter(const ScalarImagePtr &src)
        {
            _image->create(src->rows(), src->cols(), src->channels());
            
            for (int r = 0; r < src->rows(); ++r) {
                Scalar *imgRow = _image->row(r);
                
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
        FXAA::filterPixel(int row, int col, const ScalarImagePtr &img) const
        {
            const Vector rgbNW = pixel(row-1, col-1, img);
            const Vector rgbNE = pixel(row-1, col+1, img);
            const Vector rgbSW = pixel(row+1, col-1, img);
            const Vector rgbSE = pixel(row+1, col+1, img);
            const Vector rgbM =  pixel(row, col, img);
            
            const Vector luma = Vector(Scalar(0.299), Scalar(0.587), Scalar(0.114));
            Scalar lumaNW = rgbNW.dot(luma);
            Scalar lumaNE = rgbNE.dot(luma);
            Scalar lumaSW = rgbSW.dot(luma);
            Scalar lumaSE = rgbSE.dot(luma);
            Scalar lumaM = rgbM.dot(luma);
            
            Scalar lumaMin = SMIN(lumaNW, SMIN(lumaNE, SMIN(lumaSW, SMIN(lumaSE, lumaM))));
            Scalar lumaMax = SMAX(lumaNW, SMAX(lumaNE, SMAX(lumaSW, SMAX(lumaSE, lumaM))));
            
            // FXAA params - fixed here for now.
            const Scalar fxaa_reduce_mul = Scalar(1) / Scalar(32);
            const Scalar fxaa_reduce_min = Scalar(1) / Scalar(128);
            const Scalar fxaa_span_max = Scalar(8);
            
            // Find direction and blur orthogonal to edge.
            
            Vector2 dir;
            dir.x() = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
            dir.y() = ((lumaNW + lumaSW) - (lumaNE + lumaSE));
            
            Scalar dirRed = SMAX((lumaNW + lumaNE + lumaSW + lumaSE) * (Scalar(0.25) * fxaa_reduce_mul), fxaa_reduce_min);
            Scalar invDirMin = Scalar(1) / (SMIN(abs(dir.x()), abs(dir.y())) + dirRed);
            
            dir *= invDirMin;
            dir.x() = clamp(dir.x(), -fxaa_span_max, fxaa_span_max);
            dir.y() = clamp(dir.y(), -fxaa_span_max, fxaa_span_max);
            
            const Scalar c = Scalar(1)/Scalar(3) - Scalar(0.5);
            const Scalar d = Scalar(2)/Scalar(3) - Scalar(0.5);
            const Scalar e = Scalar(0)/Scalar(3) - Scalar(0.5);
            const Scalar f = Scalar(3)/Scalar(3) - Scalar(0.5);
                              
            Vector rgbA = Scalar(0.5) * (bilinearSample(row + c, col + c, img) + bilinearSample(row + d, col + d, img));
            Vector rgbB = rgbA * Scalar(0.5) + Scalar(0.25) * (bilinearSample(row + e, col + e, img) + bilinearSample(row + f, col + f, img));
            Scalar lumaB = rgbB.dot(luma);
            
            if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
                return rgbA;
            } else {
                return rgbB;
            }
        }
    }
}