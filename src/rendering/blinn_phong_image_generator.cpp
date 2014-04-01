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
        :_image(new ByteImage()), _clearColor(Vector::Zero())
        {}
        
        void
        BlinnPhongImageGenerator::onRenderingBegin(const Renderer *r)
        {
            _image->create(r->imageHeight(), r->imageWidth(), 3);
            _root = r->scene();
            _light = LightPtr(new Light());
        }
        
        
        void
        BlinnPhongImageGenerator::onUpdateRow(int row,
                                        const Vector &origin,
                                        const Vector *directions,
                                        const SDFNode::TraceResult *tr, int cols)
        {
            unsigned char *imageRow = _image->row(row);
            
            for (int c = 0; c < cols; ++c) {
                /*if (tr[c].hit) {
                    Vector p = origin + tr[c].t * directions[c];
                } else {
                    imageRow[c] = _invValue;
                }*/
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
            // http://www.cs.uregina.ca/Links/class-info/315/WWW/Lab4/
            /*
            Illumination r;
            SDFResult sdf = _root->fullEval(p);
            MaterialPtr m = sdf.node->attachment<Material>("Material");
            LightPtr l = _light;
            
            r.ambient = m->l->ambientColor() * m->ambientReflectionConstant();
            
            if (m->diffuseReflectionConstant() == 0) {
                return r;
            }
            
            Vector n = _root->normal(p);
            Vector ldir = l->position() - p;
            Scalar dist = ldir.norm();
            ldir /= dist;
            dist = dist * dist;
            
            // Diffuse term
            
            Scalar ndotl = n.dot(ldir);
            Scalar intensity = clamp(ndotl, Scalar(0), Scalar(1));
            r.diffuse = (intensity * l->diffuseColor() * m->diffuseReflectionConstant()) / dist;
            
            // Specular term
            
            Vector h = (ldir + viewDir).normalized();
            Scalar ndoth = n.dot(h);
            intensity = std::pow(clamp(ndoth, Scalar(0), Scalar(1)), m->specularHardness());
            r.specular = (intensity * l->specularColor() * m->specularReflectionConstant()) / dist;
            
            
            return r;
            */
        }
        
    }
}