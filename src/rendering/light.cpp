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
        : _position(100, 100, 100), _ambientColor(0,0,0), _diffuseColor(1,1,1), _specularColor(1,1,1)
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
        
        void
        Light::setAmbientColor(const Vector &c)
        {
            _ambientColor = c;
        }
        
        const Vector &
        Light::ambientColor() const
        {
            return _ambientColor;
        }
        
        void
        Light::setDiffuseColor(const Vector &c)
        {
            _diffuseColor = c;
        }
        
        const Vector &
        Light::diffuseColor() const
        {
            return _diffuseColor;
        }
        
        void
        Light::setSpecularColor(const Vector &c)
        {
            _specularColor = c;
        }
        
        const Vector &
        Light::specularColor() const
        {
            return _specularColor;
        }
        
    }
}