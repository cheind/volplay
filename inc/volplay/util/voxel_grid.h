// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_UTIL_VOXELGRID
#define VOLPLAY_UTIL_VOXELGRID

#include <volplay/types.h>
#include <volplay/fwd.h>
#include <vector>
#include <unordered_map>

namespace volplay {
    namespace util {
    
        /** Helper class to work with voxel grids in three dimensions. */
        class VoxelGrid {
        public:

            /** A voxel is represented by its minimum corner index. */
            typedef Index Voxel;

            /** A directed voxel edge is represented by a ordered pair of voxels. */
            typedef std::pair<Voxel, Voxel> VoxelEdge;
            
            /** Build a transformation that maps from world coordinates to voxel grid coordinates. */
            inline static AffineTransform buildWorldToLocal(const Vector &origin, const Vector &resolution)
            {
                AffineTransform a;
                a.setIdentity();
                a.scale(resolution.cwiseInverse()).translate(-origin);
                return a;
            }

            /** Transform world point to voxel grid coordinates. */
            inline static Vector worldToLocal(const AffineTransform &wl, const Vector &x)
            {
                return wl * x;
            }

            /** Transform world point to voxel it falls into. */
            inline static Index worldToVoxel(const AffineTransform &wl, const Vector &x)
            {
                Vector l = worldToLocal(wl, x);
                return Index((int)floor(l.x()),
                             (int)floor(l.y()),
                             (int)floor(l.z()));
            }

        private:
            VoxelGrid();            
        };

        /** Base class for sparse properties. */
        template<class Key, class Value, class Hasher, class Equal>
        class SparseProperty {
        public:
            typedef std::unordered_map<Key, Value, Hasher, Equal> HashMap;

            /** Create with default value that is returned when property value is not set. */
            SparseProperty(const Value &defaultValue = Value())
                :_defaultValue(defaultValue)
            {}

            Value &operator[](const Key &key)
            {
                // This will not overwrite values if key is already present.
                return _props.insert(std::make_pair(key, _defaultValue)).first->second;
            }

            bool isSet(const Key &key) const
            {
                return _props.find(key) != _props.end();
            }

            typename HashMap::const_iterator begin() const
            {
                return _props.begin();
            }

            typename HashMap::const_iterator end() const
            {
                return _props.end();
            }

        private:
            
            HashMap _props;
            Value _defaultValue;
        };

        /** Helper function to combine multiple hash values. 
            This is in accordance with boost::hash_combine. */
        template<typename T> 
        void hashCombine(size_t & seed, T const& v)
        {
            seed ^= std::hash_value(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        /** Hasher for VoxelGrid::Voxel */
        struct HashVoxel {
            inline size_t operator()(const VoxelGrid::Voxel &v) const
            {
                size_t seed = 0;
                hashCombine(seed, v.x());
                hashCombine(seed, v.y());
                hashCombine(seed, v.z());
                return seed;
            }
        };

        /** Equality for VoxelGrid::Voxel */
        struct EqualVoxel {
            inline bool operator()(const VoxelGrid::Voxel &a, const VoxelGrid::Voxel &b) const
            {
                return a.x() == b.x() &&
                        a.y() == b.y() &&
                        a.z() == b.z();
            }
        };

        /** Sparse property assigned to voxels. */
        template<class Value>
        class SparseVoxelProperty : public SparseProperty<VoxelGrid::Voxel, Value, HashVoxel, EqualVoxel> 
        {
        public:
            SparseVoxelProperty(const Value &defaultValue)
                : SparseProperty<VoxelGrid::Voxel, Value, HashVoxel, EqualVoxel>(defaultValue)
            {}
        };

        /** Hasher for VoxelGrid::VoxelEdge */
        struct HashVoxelEdge {
            inline size_t operator()(const VoxelGrid::VoxelEdge &v) const
            {
                size_t seed = 0;
                hashCombine(seed, v.first.x());
                hashCombine(seed, v.first.y());
                hashCombine(seed, v.first.z());
                hashCombine(seed, v.second.x());
                hashCombine(seed, v.second.y());
                hashCombine(seed, v.second.z());
                return seed;
            }
        };

        /** Equality for VoxelGrid::VoxelEdge */
        struct EqualVoxelEdge {
            inline bool operator()(const VoxelGrid::VoxelEdge &a, const VoxelGrid::VoxelEdge &b) const
            {
                return a.first.x() == b.first.x() &&
                       a.first.y() == b.first.y() &&
                       a.first.z() == b.first.z() &&
                       a.second.x() == b.second.x() &&
                       a.second.y() == b.second.y() &&
                       a.second.z() == b.second.z();
            }
        };

        /** Sparse property assigned to voxels. */
        template<class Value>
        class SparseVoxelEdgeProperty : public SparseProperty<VoxelGrid::VoxelEdge, Value, HashVoxelEdge, EqualVoxelEdge> 
        {
        public:
            SparseVoxelEdgeProperty(const Value &defaultValue)
                : SparseProperty<VoxelGrid::VoxelEdge, Value, HashVoxelEdge, EqualVoxelEdge>(defaultValue)
            {}
        };

       

    }
}

#endif
