// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include "catch.hpp"
#include "float_comparison.hpp"
#include <volplay/sdf_sphere.h>
#include <volplay/sdf_node_attachment.h>

namespace vp = volplay;

class SDFTestAttachment : public vp::SDFNodeAttachment {
public:    
    int id;
};

TEST_CASE("SDFNode")
{
    vp::SDFSphere s;
    std::shared_ptr<SDFTestAttachment> a = s.getOrCreateAttachment<SDFTestAttachment>("id");
    a->id = 3;
    
    REQUIRE(s.attachment<SDFTestAttachment>("id")->id == 3);
    
    std::shared_ptr<SDFTestAttachment> b = s.getOrCreateAttachment<SDFTestAttachment>("id");
    REQUIRE(a.get() == b.get());
}