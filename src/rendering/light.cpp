// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/rendering/light.h>

namespace volplay {
    
    namespace rendering {
    
        Light::Light()
        : _position(0, 0, 100)
        {}
        
        void
        Light::setPosition(const Vector &pos)
        {
            _position = pos;
        }
        
        const Vector &
        Light::position() const
        {
            return _position;
        }
        
    }
}