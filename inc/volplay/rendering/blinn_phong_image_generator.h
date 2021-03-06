// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_BLINN_PHONG_IMAGE_GENERATOR
#define VOLPLAY_BLINN_PHONG_IMAGE_GENERATOR

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <volplay/rendering/image_generator.h>
#include <volplay/sdf_node.h>
#include <vector>

namespace volplay {
    
    namespace rendering {

        /** Generates a color image using the Blinn-Phong reflection model.
         *
         *  SDFNodes need to carry a 'Material' attachment of type Material.
         */
        class BlinnPhongImageGenerator : public ImageGenerator {
        public:
            /** Default constructor */
            BlinnPhongImageGenerator();
            
            virtual void onRenderingBegin(const Renderer *r);
            virtual void onUpdateRow(int row,
                                     const Vector &origin,
                                     const Vector *directions,
                                     const SDFNode::TraceResult *tr, int cols);
            virtual void onRenderingComplete(const Renderer *r);
            
            /** Enable / disable shadow calculations. */
            void setShadowsEnabled(bool enable);
            
            /** Set gamma correction factor. Defaults to 1 / 2.2. */
            void setGamma(Scalar s);
            
            /** Enable / disable edge antialiasing. */
            void setAntialiasingEnabled(bool enable);
            
            /** Access the generated RGB color image. */
            ByteImagePtr image() const;
            
        private:
            
            /** Illuminate point. */
            Vector illuminate(const Vector &viewDir, const Vector &p) const;
            
            /** Illuminate point from a single light source. */
            Vector illuminateFromLight(const Vector &p, const Vector &normal, const Vector &eye,
                                       const MaterialPtr &m, const LightPtr &l, const SDFNode *node) const;
            
            /** Calcuate light attenuation factor. */
            Scalar calculateLightAttenuation(const Scalar &d, const LightPtr &l) const;
            
            /** Calculate shadow factor. */
            Scalar calculateSoftShadow(const Vector &origin, const Vector &dir,
                                       Scalar minT, Scalar maxT,
                                       const LightPtr &l,
                                       const SDFNode *node) const;
            
            
            ByteImagePtr _saturatedImage;
            ScalarImagePtr _image;
            SDFNodePtr _root;
            std::vector<LightPtr> _lights;
            MaterialPtr _defaultMaterial;
            Vector _clearColor;
            SDFNode::TraceOptions _to;
            Scalar _gamma;
            bool _shadowsEnabled;
            bool _fxaaEnabled;
            FXAAPtr _fxaa;
        };
        
    }
}

#endif
