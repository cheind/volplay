// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SATURATE
#define VOLPLAY_SATURATE

#include <cmath>
#include <limits>

namespace volplay {
    
    namespace rendering {

        /** Simple rounding function. Implemented here since not available in MSVC10. */
        template<class T>
        T round(double v) 
        {
            return v < 0 ? static_cast<T>(std::ceil(v - 0.5)) : static_cast<T>(v + T(0.5));
        }
        
        /** Saturate input to destination range. */
        template<class T, class S>
        T saturate(S s) 
        {
            S r = ::volplay::rendering::round<S>(s);
            r = std::min<S>(r, std::numeric_limits<T>::max());
            r = std::max<S>(r, std::numeric_limits<T>::lowest());
            return static_cast<T>(r);
        }
        
        /** Clamp value to range. */
        template<class T>
        T clamp(T t, T minimum, T maximum) {
            return std::min<T>(maximum, std::max<T>(t, minimum));
        }

        /** Clamp value to [0,1] range. */
        template<class T>
        T clamp01(T t) {
            return clamp(t, T(0), T(1));
        }
        
    }
}

#endif
