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

#include <math.h>
#include <limits>

namespace volplay {
    
    namespace rendering {
        
        /** Saturate input to destination range. */
        template<class T, class S>
        T saturate(S s) {
            S r = round(s);
            r = std::min<S>(r, std::numeric_limits<T>::max());
            r = std::max<S>(r, std::numeric_limits<T>::lowest());
            return static_cast<T>(r);
        }
        
    }
}

#endif
