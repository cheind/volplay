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
            
            /** Set ambient color of light. */
            void setAmbientColor(const Vector &c);
            
            /** Access ambient color of light. */
            const Vector &ambientColor() const;
            
            /** Set diffuse color of light. */
            void setDiffuseColor(const Vector &c);
            
            /** Access diffuse color of light. */
            const Vector &diffuseColor() const;
            
            /** Set diffuse color of light. */
            void setSpecularColor(const Vector &c);
            
            /** Access diffuse color of light. */
            const Vector &specularColor() const;
            
            /** Set light attenuation constants. That is how lighting power decreases with distance.
             *  The components of the vector specified are the coefficients of a quadric function.
             *  In particular attenuation is calculated as
             *  
             *   att = 1 / (c.x() * d^2 + c.y() * d + c.z())
             */
            void setAttenuationCoefficients(const Vector &c);
            
            /** Access the light attenuation constants. */
            const Vector &attenuationCoefficients() const;
            
            /** Create point light source. */
            static LightPtr createPointLight(const Vector &pos, const Vector &ambient, const Vector &diffuse, const Vector &specular, const Vector &att);
            
        private:
            Vector _position;
            Vector _ambientColor;
            Vector _diffuseColor;
            Vector _specularColor;
            Vector _attenuation;
        };
        
    }
}

#endif
