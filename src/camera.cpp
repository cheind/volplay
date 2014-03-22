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
    : _k(Matrix33::Identity()), _camera_to_world(Matrix34::Identity())
    {}
    
    void
    Camera::setCameraToImage(const Vector2 &focalLength, const Vector2 &principalPoint)
    {
        _k.matrix() << focalLength.x(), 0, principalPoint.x(),
                       0, focalLength.y(), principalPoint.y(),
                       0, 0, 1;
        
    }
    
    Camera::Matrix33
    Camera::cameraToImage() const
    {
        return _k;
    }
    
    void
    Camera::setCameraToWorld(const Matrix34 &t)
    {
        _camera_to_world = t;
    }
    
    void
    Camera::setCameraToWorld(const AffineTransform &t)
    {
        _camera_to_world = t.matrix().block<3,4>(0,0);
    }
    
    Camera::Matrix34
    Camera::cameraToWorld() const
    {
        return _camera_to_world;
    }
    
    Camera::Matrix34
    Camera::worldToCamera() const
    {
        const Matrix33 rinv = _camera_to_world.block<3,3>(0,0).transpose();
        const Vector tinv = -rinv * _camera_to_world.block<3,1>(0,3);
        
        Matrix34 r;
        r.block<3,3>(0,0) = rinv;
        r.block<3,1>(0,3) = tinv;
        return r;
    }
    
    Camera::Matrix34
    Camera::worldToImage() const
    {
        return _k * worldToCamera();
    }
    
    Camera::Matrix33
    Camera::imageToCamera() const
    {
        return _k.inverse();
    }
    
    Vector
    Camera::cameraOriginInWorld() const
    {
        return _camera_to_world.block<3,1>(0,3);
    }
    
    void
    Camera::generateCameraRay(const Vector2 &imagePoint, Vector &direction)
    {
        Matrix33 kinv = this->imageToCamera();
        direction = (kinv * imagePoint.homogeneous()).normalized();
    }
    
    void
    Camera::generateCameraRays(const std::vector<Vector2> &imagePoints, std::vector<Vector> &directions)
    {
        Matrix33 kinv = this->imageToCamera();
        directions.resize(imagePoints.size());
        
        for (size_t i = 0; i < imagePoints.size(); ++i) {
            directions[i] = (kinv * imagePoints[i].homogeneous()).normalized();
        }
    }
    
    void
    Camera::generateWorldRay(const Vector2 &imagePoint, Vector &direction)
    {
        Matrix33 f = _camera_to_world.block<3,3>(0, 0) * this->imageToCamera();
        direction = (f * imagePoint.homogeneous()).normalized();
    }
    
    void
    Camera::generateWorldRays(const std::vector<Vector2> &imagePoints, std::vector<Vector> &directions)
    {
        Matrix33 f = _camera_to_world.block<3,3>(0, 0) * this->imageToCamera();
        
        for (size_t i = 0; i < imagePoints.size(); ++i) {
            directions[i] = (f * imagePoints[i].homogeneous()).normalized();
        }
    }

    
}