// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_FWD
#define VOLPLAY_FWD

#include <memory>

namespace volplay {
    
    class SDFNode;
    class SDFGroup;
    
    typedef std::shared_ptr<SDFNode> SDFNodePtr;
    typedef std::shared_ptr<SDFGroup> SDFGroupPtr;
}

#endif