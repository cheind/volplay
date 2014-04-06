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
#include <iostream>

namespace volplay {
    
    namespace rendering {
    
        BlinnPhongImageGenerator::BlinnPhongImageGenerator()
        :_image(new ByteImage()), _defaultMaterial(new Material()), _clearColor(Vector::Zero())
        {
        }
        
        void
        BlinnPhongImageGenerator::onRenderingBegin(const Renderer *r)
        {
            _image->create(r->imageHeight(), r->imageWidth(), 3);
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
            unsigned char *imageRow = _image->row(row);
            
            for (int c = 0; c < cols; ++c) {
                Vector i = _clearColor;
                if (tr[c].hit) {
                    Vector p = origin + tr[c].t * directions[c];
                    i = illuminate(directions[c], p);
                }
                unsigned char r = saturate<unsigned char>(i.x() * Scalar(255));
                unsigned char g = saturate<unsigned char>(i.y() * Scalar(255));
                unsigned char b = saturate<unsigned char>(i.z() * Scalar(255));

                imageRow[c*3+0] = r;
                imageRow[c*3+1] = g;
                imageRow[c*3+2] = b;
                
            }
        }
        
        void
        BlinnPhongImageGenerator::onRenderingComplete(const Renderer *r)
        {
            // Nothing todo
        }
        
        ByteImagePtr
        BlinnPhongImageGenerator::image() const
        {
            return _image;
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
        BlinnPhongImageGenerator::illuminateFromLight(const Vector &p, const Vector &n, const Vector &eye, const MaterialPtr &m, const LightPtr &l, const SDFNode *node) const
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
            const Scalar attenuation = calculateLightAttenuation(lp, l);
            
            // Shadow factor
            const Scalar shadow = calculateSoftShadow(p, ldir, Scalar(0.001), lpNorm, node);
            
            return iAmbient + attenuation * shadow * (iDiffuse + iSpecular);
        }
        
        Scalar
        BlinnPhongImageGenerator::calculateLightAttenuation(const Vector &lightVector, const LightPtr &l) const
        {
            // Calculate the attenuation factor. Standard OpenGL uses three factors to calculate the
            // attenuation as
            //
            //  att = 1 / (c.x() * d^2 + c.y() * d + c.z())
            //
            // We use a simplier equation
            //
            //  att = 1 - (d/r)^2
            
            const Scalar attf =  clamp01(lightVector.norm() / l->attenuationRadius());
            return Scalar(1) - attf*attf;
        }
        
        Scalar
        BlinnPhongImageGenerator::calculateSoftShadow(const Vector &o, const Vector &d, Scalar minT, Scalar maxT, const SDFNode *node) const
        {
            Scalar s(1);
            Scalar t = minT;
            SDFResult r = _root->fullEval(o + t * d);
            
            while (t < maxT && r.sdf > _to.sdfThreshold) {
                t += r.sdf * _to.stepFact;
                r = _root->fullEval(o + t * d);
                s = std::min<Scalar>(s, Scalar(32) * r.sdf / t);
            }
            
            if (std::abs(r.sdf) < _to.sdfThreshold) {
                return 0;
            } else {
                return clamp01(s);
            }
            
        }
        
    }
}