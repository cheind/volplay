// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/sdf_group.h>

namespace volplay {
    
    SDFGroup::SDFNodeArray::iterator
    SDFGroup::add(const SDFNodePtr &p) {
        _nodes.push_back(p);
        return _nodes.end() - 1;
    }
    
    SDFGroup::SDFNodeArray::iterator
    SDFGroup::begin() {
        return _nodes.begin();
    }
    
    SDFGroup::SDFNodeArray::const_iterator
    SDFGroup::begin() const {
        return _nodes.begin();
    }
    
    SDFGroup::SDFNodeArray::iterator
    SDFGroup::end() {
        return _nodes.end();
    }
    
    SDFGroup::SDFNodeArray::const_iterator
    SDFGroup::end() const {
        return _nodes.end();
    }
    
    SDFGroup::SDFNodeArray::size_type
    SDFGroup::size() const
    {
        return _nodes.size();
    }

	bool
	SDFGroup::isGroup() const
	{
		return true;
	}
    
}