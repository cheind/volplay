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

        /** A light source. Currently only supports point lights. */
        class Light {
        public:
            
            /** Default initializer. */
            Light();
            
            /** Set position of light source. */
            void setPosition(const Vector &pos);
            
            /** Access the position of the light source. */
            const Vector &position() const;
            
            /** Set diffuse color of light. */
            void setDiffuseColor(const Vector &c);
            
            /** Access diffuse color of light. */
            const Vector &diffuseColor() const;
            
            /** Set diffuse color of light. */
            void setSpecularColor(const Vector &c);
            
            /** Access diffuse color of light. */
            const Vector &specularColor() const;
            
            /** Set diffuse color power. */
            void setDiffusePower(Scalar power);
            
            /** Access diffuse power. */
            Scalar diffusePower() const;
            
            /** Set specular color power. */
            void setSpecularPower(Scalar power);
            
            /** Access specular power. */
            Scalar specularPower() const;
            
        private:
            Vector _position;
            Vector _diffuseColor;
            Vector _specularColor;
            Scalar _diffusePower;
            Scalar _specularPower;
        };
        
    }
}

#endif
