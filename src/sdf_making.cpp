// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_making.h>
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_repetition.h>

namespace volplay {
    
    SDFSpherePtr
    makeSDFSphere(Scalar radius)
    {
        return SDFSpherePtr(new SDFSphere(radius));
    }
    
    SDFRepetitionPtr
    makeSDFRepetition(const Vector &cellSizes, const SDFNodePtr &node)
    {
        return SDFRepetitionPtr(new SDFRepetition(cellSizes, node));
    }
    
    
}