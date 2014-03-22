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
#include <vector>

namespace volplay {

    /** Represents a pinhole / orthographic camera model. */
    class Camera {
    public:
        /** Default type of camera intrinsic matrix. */
        typedef Eigen::Matrix<Scalar, 3, 3> Matrix33;
        
        /** Default type of matrix that performs 3D world -> 2D image projection. */
        typedef Eigen::Matrix<Scalar, 3, 4> Matrix34;
        
        /** Empty initializer. */
        Camera();
        
        /** Builds the camera intrinsic matrix using focal lengths and principal point. */
        void setCameraToImage(const Vector2 &focalLength, const Vector2 &principalPoint);
        
        /** Set camera view transform. */
        void setCameraToWorld(const Matrix34 &t);
        
        /** Set camera view transform. */
        void setCameraToWorld(const AffineTransform &t);
        
        /** Matrix that transforms 2D image points to 3D points with depth 1. */
        Matrix33 imageToCamera() const;
        
        /** Retrieve the matrix that converts 3D points in camera space to points in image space. */
        Matrix33 cameraToImage() const;
        
        /** Retrieve matrix that converts 3D world points to 3D points in camera space. */
        Matrix34 worldToCamera() const;
        
        /** Retrieve camera view transform, i.e how the camera is positioned in world space. */
        Matrix34 cameraToWorld() const;
        
        /** Matrix that transforms 3D points to 2D image points. */
        Matrix34 worldToImage() const;
        
        /** Returns the camera origin in world space. */
        Vector cameraOriginInWorld() const;
        
        /** Generate normalized ray direction through given image point in camera space. */
        void generateCameraRay(const Vector2 &imagePoint, Vector &direction);
        
        /** Generate normalized ray directions through given image points in camera space. */
        void generateCameraRays(const std::vector<Vector2> &imagePoints, std::vector<Vector> &directions);
        
        /** Generate normalized ray direction through given image point in world space. */
        void generateWorldRay(const Vector2 &imagePoint, Vector &direction);
        
        /** Generate normalized ray directions through given image points in world space. */
        void generateWorldRays(const std::vector<Vector2> &imagePoints, std::vector<Vector> &directions);

    private:
        Matrix33 _k;
        Matrix34 _camera_to_world;
    };

}

#endif
