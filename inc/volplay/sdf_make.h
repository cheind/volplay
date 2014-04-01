// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_MAKE
#define VOLPLAY_SDF_MAKE

#include <volplay/types.h>
#include <volplay/fwd.h>

namespace volplay {
    
    /** Helps in making complex scene graphs with a syntax similar to plane().join(sphere()). */
    class SDFMake {
    public:
        explicit SDFMake(SDFNodePtr n);
        
        SDFMake &join(const SDFMake &right);
        SDFMake &intersect(const SDFMake &right);
        SDFMake &remove(const SDFMake &right);
        SDFMake &translate(const Vector &t);
        SDFMake &repeat(const Vector &cellSizes);
        SDFMake &attach(const std::string &s, const SDFNodeAttachmentPtr &a);
        
        operator SDFNodePtr() const;
        
        /** Encapsulate exisiting node. */
        static SDFMake wrap(const SDFNodePtr &n);
        
        /** Make a new sphere. */
        static SDFMake sphere(Scalar radius);
        
        /** Make a new plane. */
        static SDFMake plane();
        
    private:
        SDFNodePtr _n;
    };
    
}

#endif
