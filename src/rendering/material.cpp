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
        : _ambientColor(0,0,0)
        , _diffuseColor(1,1,1)
        , _specularColor(1,1,1)
        , _ambientConstant(1)
        , _diffuseConstant(1)
        , _specularConstant(1)
        , _specularHardness(32)
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
        
        void
        Material::setAmbientReflectionConstant(Scalar s)
        {
            _ambientConstant = s;
        }
        
        Scalar
        Material::ambientReflectionConstant() const
        {
            return _ambientConstant;
        }
        
        void
        Material::setDiffuseReflectionConstant(Scalar s)
        {
            _diffuseConstant = s;
        }
        
        Scalar
        Material::diffuseReflectionConstant() const
        {
            return _diffuseConstant;
        }
        
        void
        Material::setSpecularReflectionConstant(Scalar s)
        {
            _specularConstant = s;
        }
        
        Scalar
        Material::specularReflectionConstant() const
        {
            return _specularConstant;
        }
        
    }
}