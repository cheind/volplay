// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_REPETITION
#define VOLPLAY_SDF_REPETITION

#include <volplay/types.h>
#include <volplay/sdf_unary.h>

namespace volplay {

    /** Repeats a node along one or more coordinate axis. */
    class SDFRepetition : public SDFUnary {
    public:
        /** Create an empty repetition node. */
        SDFRepetition();
        
        /** Create repetition node with cell sizes. */
        SDFRepetition(const Vector &cellSizes);
        
        /** Create repetition node with cell sizes and affected node. */
        SDFRepetition(const Vector &cellSizes, const SDFNodePtr &n);
        
        /** Access the cell sizes. */
        const Vector &cellSizes() const;
        
        /** Set new cell sizes. */
        void setCellSizes(const Vector &cellSizes);
        
        /** Evaluate the SDF at given position. */
        virtual SDFResult fullEval(const Vector &x) const;
    private:
        Vector _cellSizes;
    };

}

#endif
