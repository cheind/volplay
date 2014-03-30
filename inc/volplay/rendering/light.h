// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_LIGHT
#define VOLPLAY_LIGHT

#include <volplay/types.h>
#include <volplay/fwd.h>

namespace volplay {
    
    namespace rendering {

        /** A light source. */
        class Light {
        public:
            
            /** Default initializer. */
            Light();
            
            /** Set position of light source. */
            void setPosition(const Vector &pos);
            
            /** Access the position of the light source. */
            const Vector &position() const;
            

        private:
            Vector _position;
        };
        
    }
}

#endif
