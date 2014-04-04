// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/rendering/camera.h>

namespace volplay {
    
    namespace rendering {
    
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
        
        void
        Camera::setCameraToImage(int imageHeight, int imageWidth, Scalar hfov_radians, Scalar vfov_radians)
        {
            Scalar fx = imageWidth / (Scalar(2) * tan(hfov_radians * Scalar(0.5)));
            Scalar fy = imageHeight / (Scalar(2) * tan(vfov_radians * Scalar(0.5)));
            
            _k.matrix() << fx, 0, imageWidth * Scalar(0.5),
            0, fy, imageHeight * Scalar(0.5),
            0, 0, 1;
        }
        
        void
        Camera::setCameraToImage(int imageHeight, int imageWidth, Scalar vfov_radians)
        {
            Scalar aspect = Scalar(imageWidth) / Scalar(imageHeight);
            Scalar sy = Scalar(1) / tan(vfov_radians * Scalar(0.5));
            Scalar sx = sy / aspect;
            
            Scalar fy = imageHeight * sy / Scalar(2);
            Scalar fx = imageWidth * sx / Scalar(2);
            
            _k.matrix() << fx, 0, imageWidth * Scalar(0.5),
            0, fy, imageHeight * Scalar(0.5),
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
        
        void
        Camera::setCameraToWorldAsLookAt(const Vector &eye, const Vector &center, const Vector &up)
        {
            Vector z = (center - eye).normalized();
            Vector y = -up.normalized(); // Note our up is -y.
            Vector x = (y.cross(z)).normalized();
            y = z.cross(x);
            
            _camera_to_world = Matrix34::Identity();
            _camera_to_world << x.x(), y.x(), z.x(), eye.x(),
                                x.y(), y.y(), z.y(), eye.y(),
                                x.z(), y.z(), z.z(), eye.z();
        }
        
        Camera::Matrix34
        Camera::cameraToWorld() const
        {
            return _camera_to_world;
        }
        
        AffineTransform
        Camera::cameraToWorldTransform() const
        {
            AffineTransform at = AffineTransform::Identity();
            at.matrix().block<3,4>(0,0) = _camera_to_world;
            return at;
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

        AffineTransform 
        Camera::worldToCameraTransform() const
        {
            AffineTransform at = AffineTransform::Identity();
            at.matrix().block<3,4>(0,0) = worldToCamera();
            return at;
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
        Camera::originInWorld() const
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
        Camera::generateCameraRays(int imageHeight, int imageWidth, std::vector<Vector> &directions)
        {
            Matrix33 kinv = this->imageToCamera();  
            directions.resize(imageWidth * imageHeight);
            
            for (int r = 0; r < imageHeight; ++r) {
                for (int c = 0; c < imageWidth; ++c) {
                    directions[r * imageWidth + c] = (kinv * Vector2(c, r).homogeneous()).normalized();
                }
            }
        }
        
    }
}