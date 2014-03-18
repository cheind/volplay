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
    
    /** Make a new sphere. */
    SDFSpherePtr makeSDFSphere(Scalar radius);
    
    /** Repeat object along axis. */
    SDFRepetitionPtr makeSDFRepetition(const Vector &cellSizes, const SDFNodePtr &node = SDFNodePtr());

}

#endif
