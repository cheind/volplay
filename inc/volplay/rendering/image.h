// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_IMAGE
#define VOLPLAY_IMAGE

#include <volplay/types.h>
#include <volplay/fwd.h>

#ifdef VOLPLAY_WITH_OPENCV
#include <opencv2/core/core.hpp>
#endif

namespace volplay {
    
    namespace rendering {

        /** A two-dimensional image. */
        template<class T>
        class Image {
        public:
            /** Create empty image. */
            Image()
            : _data(0), _rows(0), _cols(0), _channels(0)
            {}
            
            /** Create image with known dimensions. */
            Image(int rows, int cols, int channels)
            :_data(0)
            {
                create(rows, cols, channels);
            }
            
            /** Deallocate image. */
            ~Image()
            {
                create(0,0,0);
            }
            
            /** Allocate image memory. */
            void create(int rows, int cols, int channels)
            {
                const int oldSizeInT = _rows * _cols * _channels;
                const int newSizeInT = rows * cols * channels;
                
                _rows = rows;
                _cols = cols;
                _channels = channels;
                
                if (oldSizeInT == newSizeInT) {
                    return;
                }
                
                if (_data != 0) {
                    delete [] _data;
                    _data = 0;
                }
                
                
                if (newSizeInT > 0) {
                    _data = new T[newSizeInT];
                }
            }
            
            /** Copy image content */
            void copyTo(Image<T> &dst) {
                const int sizeInBytes = _rows * _cols * _channels * sizeof(T);
                if (sizeInBytes > 0) {
                    dst.create(rows(), cols(), channels());
                    memcpy(dst.row(0), row(0), sizeInBytes);
                }
            }
            
            /** Return the number of rows. */
            int rows() const
            {
                return _rows;
            }
            
            /** Return the number of columns. */
            int cols() const
            {
                return _cols;
            }
            
            /** Return the number of channels. */
            int channels() const
            {
                return _channels;
            }
            
            /** Access the i-th row. */
            T *row(int index) {
                return _data + _cols * _channels * index;
            }
            
            /** Access element of row. */
            T *rowElement(T * row, int col)
            {
                return row + col * _channels;
            }
            
            /** Access element of row mapped as Eigen vector type. */
            template<class EigenType>
            Eigen::Map<EigenType> mappedRowElement(T * row, int col)
            {
                return Eigen::Map<EigenType>(rowElement(row, col));
            }
            
#ifdef VOLPLAY_WITH_OPENCV
            cv::Mat toOpenCV() {
                return cv::Mat(_rows, _cols, CV_MAKETYPE(cv::DataType<T>::depth, _channels), (char*)_data);
            }
#endif
         
        private:
            T *_data;
            int _rows, _cols, _channels;
        };
        
    }
}

#endif
