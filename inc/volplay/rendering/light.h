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
            
            /** Set light attenuation radius. Lighting power will decrease within the radius
             *  according to some equation. */
            void setAttenuationRadius(Scalar s);
            
            /** Access the light attenuation radius. */
            Scalar attenuationRadius() const;
            
            /** Create point light source. */
            static LightPtr createPointLight(const Vector &pos, const Vector &ambient, const Vector &diffuse, const Vector &specular, const Scalar &att);
            
        private:
            Vector _position;
            Vector _ambientColor;
            Vector _diffuseColor;
            Vector _specularColor;
            Scalar _attenuation;
        };
        
    }
}

#endif
