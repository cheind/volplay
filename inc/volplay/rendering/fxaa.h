// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_FXAA
#define VOLPLAY_FXAA

#include <volplay/types.h>
#include <volplay/fwd.h>

namespace volplay {
    
    namespace rendering {

        /** Fast approximate anti-aliasing.
         *
         *  Based on http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf
         */
        class FXAA {
        public:
            
            /** Default constructor */
            FXAA();
            
            /** Perform anti-aliasing on 3-channel RGB. */
            ByteImagePtr filter(const ByteImagePtr &img);
            
        private:
            
            /** Filter a single pixel using FXAA */
            Vector filterPixel(int row, int col, const ByteImagePtr &img) const;
            
            ByteImagePtr _image;                           
        };
        
    }
}

#endif
