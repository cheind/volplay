// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_SDF_GROUP
#define VOLPLAY_SDF_GROUP

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <volplay/sdf_node.h>

#include <vector>
#include <memory>

namespace volplay {

    /** An abstract non-leaf node in the SDF scene graph with aggregation possibilities. */
    class SDFGroup : public SDFNode {
    public:
        /** Type used internally to hold nodes. */
        typedef std::vector< SDFNodePtr > SDFNodeArray;
        
        /** Add a new node */ 
        virtual SDFNodeArray::iterator add(const SDFNodePtr &n);
        
        /** Iterator to beginning of collection */
        SDFNodeArray::iterator begin();
        
        /** Iterator to beginning of collection */
        SDFNodeArray::const_iterator begin() const;
        
        /** Iterator to end of collection */
        SDFNodeArray::iterator end();
        
        /** Iterator to end of collection */
        SDFNodeArray::const_iterator end() const;
        
        /** Returns the number of children in this group */
        SDFNodeArray::size_type size() const;

		/* Test if this node is able to group other nodes. */
		virtual bool isGroup() const;

		
    private:
        SDFNodeArray _nodes;
    };

}

#endif
