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

namespace volplay {
    
    namespace rendering {
    
        BlinnPhongImageGenerator::BlinnPhongImageGenerator()
        :_image(new ByteImage()), _clearColor(Vector::Zero()), _light(new Light()), _defaultMaterial(new Material())
        {
        }
        
        void
        BlinnPhongImageGenerator::onRenderingBegin(const Renderer *r)
        {
            _image->create(r->imageHeight(), r->imageWidth(), 3);
            _root = r->scene();
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
            LightPtr l = _light;

            // Ambient illumination
            Vector iAmbient = m->ambientColor().cwiseProduct(l->ambientColor());

            // Diffuse illumination
            Vector n = _root->normal(p);
            Vector ldir = (l->position() - p).normalized();
            Vector iDiffuse = clamp01(ldir.dot(n)) * m->diffuseColor().cwiseProduct(l->diffuseColor());

            // Specular illumination
            Vector edir = -viewDir;
            Vector h = (ldir + edir).normalized();
            Vector iSpecular = std::pow(clamp01(n.dot(h)), m->specularHardness()) * m->specularColor().cwiseProduct(l->specularColor());

            return iAmbient + iDiffuse + iSpecular;
        }
        
    }
}