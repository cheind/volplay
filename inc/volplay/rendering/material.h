// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_MATERIAL
#define VOLPLAY_MATERIAL

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <volplay/sdf_node_attachment.h>

namespace volplay {
    
    namespace rendering {

        /** Object material. */
        class Material : public SDFNodeAttachment {
        public:
            
            /** Default initializer. */
            Material();
            
            /** Set materials specular hardness. Good values are between 32 and 64. */
            void setSpecularHardness(const Scalar &h);
            
            /** Access diffuse color of light. */
            Scalar specularHardness() const;
            
            /** Ambient color of material. */
            void setAmbientColor(const Vector &ambient);
            
            /** Access diffuse color of light. */
            const Vector &ambientColor() const;
            
            /** Diffuse color of material. */
            void setDiffuseColor(const Vector &diffuse);
            
            /** Diffuse color of material. */
            const Vector &diffuseColor() const;
            
            /** Specular color of material. */
            void setSpecularColor(const Vector &specular);
            
            /** Specular color of material. */
            const Vector &specularColor() const;          

        private:
            Vector _ambientColor;
            Vector _diffuseColor;
            Vector _specularColor;
            Scalar _specularHardness;
        };
        
    }
}

#endif
