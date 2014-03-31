// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_FWD
#define VOLPLAY_FWD

#include <memory>

namespace volplay {
    
    class SDFNode;
    class SDFNodeAttachment;
    class SDFGroup;
    class SDFUnion;
    class SDFIntersection;
    class SDFDifference;
    class SDFRigidTransform;
    class SDFRepetition;
    class SDFSphere;
    class SDFPlane;
    
    typedef std::shared_ptr<SDFNode> SDFNodePtr;
    typedef std::shared_ptr<SDFNodeAttachment> SDFNodeAttachmentPtr;
    typedef std::shared_ptr<SDFGroup> SDFGroupPtr;
    typedef std::shared_ptr<SDFUnion> SDFUnionPtr;
    typedef std::shared_ptr<SDFIntersection> SDFIntersectionPtr;
    typedef std::shared_ptr<SDFDifference> SDFDifferencePtr;
    typedef std::shared_ptr<SDFRigidTransform> SDFRigidTransformPtr;
    typedef std::shared_ptr<SDFRepetition> SDFRepetitionPtr;
    typedef std::shared_ptr<SDFSphere> SDFSpherePtr;
    typedef std::shared_ptr<SDFPlane> SDFPlanePtr;
    
    typedef std::shared_ptr<SDFNode const> SDFNodeConstPtr;
    typedef std::shared_ptr<SDFGroup const> SDFGroupConstPtr;
    
    namespace rendering {
        
        class Camera;
        class Renderer;
        class ImageGenerator;
        class HeatImageGenerator;
        class DepthImageGenerator;
        
        template<class T> class Image;
        typedef Image<unsigned char> ByteImage;
        typedef Image<float> FloatImage;
        
        
        typedef std::shared_ptr<Camera> CameraPtr;
        typedef std::shared_ptr<Renderer> RendererPtr;
        typedef std::weak_ptr<Renderer> RendererWeakPtr;
        typedef std::shared_ptr<ImageGenerator> ImageGeneratorPtr;
        typedef std::shared_ptr<HeatImageGenerator> HeatImageGeneratorPtr;
        typedef std::shared_ptr<DepthImageGenerator> DepthImageGeneratorPtr;
        typedef std::shared_ptr<ByteImage> ByteImagePtr;
        typedef std::shared_ptr<FloatImage> FloatImagePtr;
    }
    
}

#endif