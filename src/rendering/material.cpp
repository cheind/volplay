// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/rendering/material.h>

namespace volplay {
    
    namespace rendering {
    
        Material::Material()
        : _ambientColor(Scalar(0.1),Scalar(0.1),Scalar(0.1))
        , _diffuseColor(Scalar(0.6),Scalar(0.6),Scalar(0.6))
        , _specularColor(Scalar(0.1),Scalar(0.1),Scalar(0.1))
        , _specularHardness(Scalar(128))
        {}
        
        void
        Material::setAmbientColor(const Vector &ambient)
        {
            _ambientColor = ambient;
        }
        
        const Vector &
        Material::ambientColor() const
        {
            return _ambientColor;
        }
        
        void
        Material::setSpecularHardness(const Scalar &h)
        {
            _specularHardness = h;
        }
        
        Scalar
        Material::specularHardness() const
        {
            return _specularHardness;
        }
        
        void
        Material::setDiffuseColor(const Vector &diffuse)
        {
            _diffuseColor = diffuse;
        }
        
        const Vector &
        Material::diffuseColor() const
        {
            return _diffuseColor;
        }
        
        void
        Material::setSpecularColor(const Vector &specular)
        {
            _specularColor = specular;
        }
        
        const Vector &
        Material::specularColor() const
        {
            return _specularColor;
        }
        
    }
}