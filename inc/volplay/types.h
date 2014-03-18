// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_TYPES
#define VOLPLAY_TYPES

#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace volplay {
    
    /** Default precision */
    typedef float Scalar;
    
    /** Shortcut for scalar */
    typedef Scalar S;
    
    /** Default vector type in three dimensions */
    typedef Eigen::Matrix<Scalar, 3, 1> Vector;
    
    /** Default vector type in two dimensions */
    typedef Eigen::Matrix<Scalar, 2, 1> Vector2;
    
    /** Affine transform in three dimensions. */
    typedef Eigen::Matrix<Scalar, 3, 4> Matrix34;
    
    /** Affine transform in three dimensions. */
    typedef Eigen::Matrix<Scalar, 3, 3> Matrix33;
    
    /** Affine transform in three dimensions. */
    typedef Eigen::Transform<Scalar, 3, Eigen::AffineCompact> AffineTransform;
    
}

#endif
