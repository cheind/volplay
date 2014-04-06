// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/rendering/blinn_phong_image_generator.h>
#include <volplay/sdf_node.h>
#include <volplay/rendering/renderer.h>
#include <volplay/rendering/image.h>
#include <volplay/rendering/saturate.h>
#include <volplay/rendering/light.h>
#include <volplay/rendering/material.h>
#include <volplay/rendering/fxaa.h>

namespace volplay {
    
    namespace rendering {
    
        BlinnPhongImageGenerator::BlinnPhongImageGenerator()
        : _saturatedImage(new ByteImage())
        , _image(new FloatImage())
        , _defaultMaterial(new Material())
        , _clearColor(Vector::Zero())
        , _gamma(1 / Scalar(2.2))
        , _shadowsEnabled(true)
        , _fxaaEnabled(true)
        , _fxaa(new FXAA())
        {
        }
        
        void
        BlinnPhongImageGenerator::onRenderingBegin(const Renderer *r)
        {
            _image->create(r->imageHeight(), r->imageWidth(), 3);
            _saturatedImage->create(r->imageHeight(), r->imageWidth(), 3);
            _root = r->scene();
            _lights = r->lights();
            _to = r->primaryTraceOptions();
        }

        
        void
        BlinnPhongImageGenerator::onUpdateRow(int row,
                                        const Vector &origin,
                                        const Vector *directions,
                                        const SDFNode::TraceResult *tr, int cols)
        {
            typedef Eigen::Map<Vector> MVector;
            float *imageRow = _image->row(row);
            
            for (int c = 0; c < cols; ++c) {
                
                MVector i = imageRow + c*3;
                
                // Illuminate
                i = _clearColor;
                if (tr[c].hit) {
                    Vector p = origin + tr[c].t * directions[c];
                    i = illuminate(directions[c], p);
                }
                
                // Perform gamma correction
                i = i.array().pow(_gamma);
            }
        }
        
        void
        BlinnPhongImageGenerator::onRenderingComplete(const Renderer *r)
        {
            typedef Eigen::Map<Vector> InVector;
            
            // Antialiase
            if (_fxaaEnabled) {
                FloatImagePtr fxaaImage = _fxaa->filter(_image);
                fxaaImage->copyTo(*_image);
            }
            
            // convert to saturated RGB
            for (int r = 0; r < _image->rows(); ++r) {
                float *inRow = _image->row(r);
                unsigned char *outRow = _saturatedImage->row(r);
                
                for (int c = 0; c < _image->cols(); ++c) {
                    InVector i = inRow + c*3;
                    
                    unsigned char r = saturate<unsigned char>(i.x() * Scalar(255));
                    unsigned char g = saturate<unsigned char>(i.y() * Scalar(255));
                    unsigned char b = saturate<unsigned char>(i.z() * Scalar(255));
                    
                    outRow[c*3+0] = r;
                    outRow[c*3+1] = g;
                    outRow[c*3+2] = b;
                }
            }
        }
        
        ByteImagePtr
        BlinnPhongImageGenerator::image() const
        {
            return _saturatedImage;
        }
        
        void
        BlinnPhongImageGenerator::setShadowsEnabled(bool enable)
        {
            _shadowsEnabled = true;
        }
        
        void
        BlinnPhongImageGenerator::setGamma(Scalar s)
        {
            _gamma = s;
        }
        
        void
        BlinnPhongImageGenerator::setAntialiasingEnabled(bool enable)
        {
            _fxaaEnabled = enable;
        }

        
        Vector
        BlinnPhongImageGenerator::illuminate(const Vector &viewDir, const Vector &p) const
        {
            // Note that illumination is performed in world space. I.e inputs are
            // w.r.t to world

            // http://www.cs.uregina.ca/Links/class-info/315/WWW/Lab4/

            SDFResult sdf = _root->fullEval(p);
            MaterialPtr m = sdf.node->attachmentOrDefault<Material>("Material", _defaultMaterial);
            
            Vector iFinal = Vector::Zero();
            
            Vector n = _root->normal(p);
            Vector eye = -viewDir;
            
            for (size_t i = 0; i < _lights.size(); ++i) {
                iFinal += illuminateFromLight(p, n, eye, m, _lights[i], sdf.node);
            }
            
            return iFinal;
        }
        
        Vector
        BlinnPhongImageGenerator::illuminateFromLight(const Vector &p, const Vector &n, const Vector &eye,
                                                      const MaterialPtr &m, const LightPtr &l, const SDFNode *node) const
        {
            const Vector lp = (l->position() - p);
            const Scalar lpNorm = lp.norm();
            const Vector ldir = lp / lpNorm;
            
            // Ambient illumination
            const Vector iAmbient = m->ambientColor().cwiseProduct(l->ambientColor());
            
            // Diffuse illumination
            const Vector iDiffuse = clamp01(ldir.dot(n)) * m->diffuseColor().cwiseProduct(l->diffuseColor());
            
            // Specular illumination
            const Vector h = (ldir + eye).normalized();
            const Vector iSpecular = std::pow(clamp01(n.dot(h)), m->specularHardness()) * m->specularColor().cwiseProduct(l->specularColor());
            
            // Light attenuation factor
            const Scalar attenuation = calculateLightAttenuation(lpNorm, l);
            
            // Shadow factor. Note tracing is done from light to intersection. See function for notes.
            const Scalar shadow = _shadowsEnabled ? calculateSoftShadow(l->position(), -ldir, 0, lpNorm, l, node) : Scalar(1);
            
            return iAmbient + attenuation * shadow * (iDiffuse + iSpecular);
        }
        
        Scalar
        BlinnPhongImageGenerator::calculateLightAttenuation(const Scalar &d, const LightPtr &l) const
        {
            // Calculate the attenuation factor. Standard OpenGL uses three factors to calculate the
            // attenuation as
            //
            //  att = 1 / (c.x() * d^2 + c.y() * d + c.z())
            //
            // We use the following equation based on "Learning Modern 3D Graphics Programming"
            // 
            //  att = 1 / (1 + k * d^2)
            //  k = 1 / r^2
            //
            // Where r is the distance where half of the light intensity is lost.
            
            const Scalar k =  Scalar(1) / (l->attenuationRadius() * l->attenuationRadius());
            return clamp01(Scalar(1) / (Scalar(1) + k * d * d));
        }
        
        Scalar
        BlinnPhongImageGenerator::calculateSoftShadow(const Vector &o, const Vector &d,
                                                      Scalar minT, Scalar maxT,
                                                      const LightPtr &l,
                                                      const SDFNode *node) const
        {
            // Note the shadow ray is traced from the light source to the intersection point. This is done to avoid offsetting the
            // ray, so that it can escape from the surface (rather difficult to come up with a single good value).
            // For reference see http://www.iquilezles.org/www/articles/rmshadows/rmshadows.htm
            
            Scalar s(1);
            Scalar t = minT;
            SDFResult r = _root->fullEval(o + t * d);
            
            while (t < maxT && r.sdf > _to.sdfThreshold) {
                s = std::min<Scalar>(s, l->shadowHardness() * r.sdf / (maxT - t));
                t += r.sdf * _to.stepFact;
                r = _root->fullEval(o + t * d);
            }
            
            if (t < maxT && r.node != node) {
                // Hit something, but not the node of the intersection.
                return 0;
            } else {
                return clamp01(s);
            }
            
        }
        
    }
}