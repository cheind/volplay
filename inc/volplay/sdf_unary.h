// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_UNARY
#define VOLPLAY_SDF_UNARY

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <volplay/sdf_node.h>

namespace volplay {

    /** An abstract non-leaf node in the SDF scene graph affecting a single child node. */
    class SDFUnary : public SDFNode {
    public:
        /** Empty initialize */
        SDFUnary();
        
        /** Initialize from node */
        SDFUnary(const SDFNodePtr &n);
        
        /** Get node */
        const SDFNodePtr &node() const;
        
        /** Set node */
        void setNode(const SDFNodePtr &n);

    private:
        SDFNodePtr _n;
    };

}

#endif
