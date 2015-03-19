// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_OFF_EXPORT
#define VOLPLAY_OFF_EXPORT

#include <volplay/types.h>
#include <volplay/surface/types.h>
#include <volplay/fwd.h>

namespace volplay {

    namespace surface {

        /** 
            Export surfaces in .OFF file format.

            Based on the description from
            http://shape.cs.princeton.edu/benchmark/documentation/off_format.html
        */
        class OFFExport {
        public:
            /** Empty initializer. */
            OFFExport();
        
            /** Export Surface */
            bool exportSurface(const char *filename, const IndexedSurface &s) const;
        };

    }
}

#endif
