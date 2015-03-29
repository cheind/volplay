// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_DISPLACEMENT
#define VOLPLAY_SDF_DISPLACEMENT

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <volplay/sdf_union.h>

namespace volplay {

    /** Computes a scalar displacement value and adds it to the SDF. */
    class SDFDisplacement : public SDFUnion {
    public:
        /** Empty transform initializer. */
        SDFDisplacement();
        
        /** Initialize from transform */
        SDFDisplacement(const ScalarFnc &fnc);
        
        /** Evaluate the SDF at given position. */
        virtual SDFResult fullEval(const Vector &x) const;
        
        /** Set displacement function */
        void setDisplacementFunction(const ScalarFnc &fnc);

		/* Accept a node visitor. */
		virtual void accept(SDFNodeVisitor &nv);
        
    private:
        ScalarFnc _dfnc;
    };

}

#endif
