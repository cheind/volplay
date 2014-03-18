// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_CAMERA
#define VOLPLAY_CAMERA

#include <volplay/types.h>
#include <volplay/fwd.h>

namespace volplay {

    /** Represents a pinhole camera model. */
    class Camera {
    public:
        /** Empty union initializer. */
        Camera();
        
        /** Set image dimensions. */
        void setImageDimensions(int width, int height);

        /** Set projection parameters using focal length and principal point. */
        void setProjectionIntrinsics(const Vector2 &focalLength, const Vector2 &principalPoint);

    private:
        Matrix33 _proj;             // The camera projection matrix
        Matrix34 _cameraToWorld;    // Camera in world
        int _width, _height;        // Width and height of image plane (i.e number of pixels).
    };

}

#endif
