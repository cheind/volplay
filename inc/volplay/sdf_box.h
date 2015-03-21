// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2015 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_BOX
#define VOLPLAY_SDF_BOX

#include <volplay/types.h>
#include <volplay/sdf_node.h>

namespace volplay {

    /** An axis aligned box primitive located at origin. */
    class SDFBox : public SDFNode {
    public:
        /** Create a new unit cube. */
        SDFBox();
        
        /** Create a box with custom size given as half extensions. */
        SDFBox(const Vector &halfExt);
        
        /** Evaluate the SDF at given position. */
        virtual SDFResult fullEval(const Vector &x) const;
    private:
        Vector _hext;
    };

}

#endif
