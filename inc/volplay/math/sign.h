// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_MATH_SIGN
#define VOLPLAY_MATH_SIGN

namespace volplay {
    namespace math {
        
        /** 
            Compute the sign of value. 

            sign(x) is 1 when x is positive,
            sign(x) is −1 when x is negative,
            sign(x) is 0 when x is zero
        */
        template <typename T> int sign(T val) {
            return (T(0) < val) - (val < T(0));
        }
    }
}

#endif
