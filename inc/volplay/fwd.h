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
    class SDFGroup;
    class SDFUnion;
    class SDFIntersection;
    class SDFDifference;
    class SDFRigidTransform;
    class SDFRepetition;
    class SDFSphere;
    class SDFPlane;
    
    typedef std::shared_ptr<SDFNode> SDFNodePtr;
    typedef std::weak_ptr<SDFNode> SDFNodeWeakPtr;
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
        
        typedef std::shared_ptr<Camera> CameraPtr;
    }
    
}

#endif