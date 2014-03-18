// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/camera.h>

namespace volplay {
    
    Camera::Camera()
    : _proj(Matrix33::Identity()), _cameraToWorld(Matrix34::Identity()), _width(0), _height(0)
    {}
    
}