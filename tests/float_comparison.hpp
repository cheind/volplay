// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the MPL was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_TESTS_FLOAT_COMPARISON
#define VOLPLAY_TESTS_FLOAT_COMPARISON

#define REQUIRE_CLOSE(left, right) \
    REQUIRE( fabs((left) - (right)) < 0.001 )

#define REQUIRE_CLOSE_VECTOR(left, right) \
    REQUIRE( ((left) - (right)).squaredNorm() < 0.001 )

#endif