// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_MAKING
#define VOLPLAY_SDF_MAKING

#include <volplay/types.h>
#include <volplay/fwd.h>

namespace volplay {
    
    namespace making {
    
        /** Helps in making complex scene graphs with a syntax similar to plane().join(sphere()). */
        class MakeWrapper {
        public:
            explicit MakeWrapper(SDFNodePtr n);
            
            MakeWrapper &join(const MakeWrapper &right);
            MakeWrapper &intersect(const MakeWrapper &right);
            MakeWrapper &remove(const MakeWrapper &right);
            MakeWrapper &translate(const Vector &t);
            MakeWrapper &repeat(const Vector &cellSizes);
            
            operator SDFNodePtr() const;
            
        private:
            SDFNodePtr _n;
        };
        
        /** Encapsulate exisiting node. */
        MakeWrapper wrap(const SDFNodePtr &n);
        
        /** Make a new sphere. */
        MakeWrapper sphere(Scalar radius);
        
        /** Make a new plane. */
        MakeWrapper plane();
    }
}

#endif
