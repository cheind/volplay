// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/surface/off_export.h>
#include <volplay/surface/indexed_surface.h>
#include <stdio.h>
#include <memory>

namespace volplay {
    
    namespace surface {

        OFFExport::OFFExport()
        {}

        bool OFFExport::exportSurface(const char *filename, const IndexedSurface &s) const
        {            
            FILE *f;

            f = fopen(filename, "w");

            if (f == 0)
                return false;

            // Header
            fprintf(f, "OFF %ld %ld %ld\n", s.vertices.cols(), s.faces.cols(), long(0));

            // Vertices
            for (IndexedSurface::VertexMatrix::Index i = 0; i < s.vertices.cols(); ++i) {
                fprintf(f, "%f %f %f\n", s.vertices(0, i), s.vertices(1, i), s.vertices(2, i));
            }

            // Faces. Either quads or triangles are supported.
            if (s.faces.rows() == 4) {
                for (IndexedSurface::FaceMatrix::Index i = 0; i < s.faces.cols(); ++i) {
                    fprintf(f, "4 %ld %ld %ld %ld\n", s.faces(0, i), s.faces(1, i), s.faces(2, i), s.faces(3, i));
                }
            } else if (s.faces.rows() == 3) {
                for (IndexedSurface::FaceMatrix::Index i = 0; i < s.faces.cols(); ++i) {
                    fprintf(f, "3 %ld %ld %ld\n", s.faces(0, i), s.faces(1, i), s.faces(2, i));
                }
            }

            fclose(f);
            return true;
        }

    }
}