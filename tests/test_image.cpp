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
#include <volplay/rendering/image.h>

namespace vpr = volplay::rendering;
namespace vp = volplay;

TEST_CASE("Image")
{
    vpr::Image<int> i;
    REQUIRE(i.rows() == 0);
    REQUIRE(i.cols() == 0);
    REQUIRE(i.channels() == 0);
    
    i.create(2, 3, 1);
    REQUIRE(i.row(1) == (i.row(0) + 3));
    REQUIRE(i.row(2) == (i.row(0) + 6));
    
    int *old = i.row(0);
    
    i.create(3, 2, 1);
    REQUIRE(old == i.row(0));
    REQUIRE(i.rows() == 3);
    REQUIRE(i.cols() == 2);
    REQUIRE(i.channels() == 1);
    
    i.create(3, 2, 2);
    REQUIRE(old != i.row(0));
    REQUIRE(i.row(1) == (i.row(0) + 4));
    REQUIRE(i.row(2) == (i.row(0) + 8));
    REQUIRE(i.rowElement(i.row(1), 0) == (i.row(0) + 4));
    REQUIRE(i.rowElement(i.row(1), 1) == (i.row(0) + 4 + 2));
    
    // Mapped type tests
    REQUIRE(i.mappedRowElement<Eigen::Vector2i>(i.row(1), 1).data() == (i.row(0) + 4 + 2));
    
#ifdef VOLPLAY_WITH_OPENCV
    cv::Mat m = i.toOpenCV();
    REQUIRE(m.rows == i.rows());
    REQUIRE(m.cols == i.cols());
    REQUIRE(m.channels() == i.channels());
    REQUIRE(m.type() == CV_32SC2);
#endif
}

