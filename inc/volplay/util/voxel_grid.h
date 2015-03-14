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
        namespace voxelgrid {

            /** A voxel is represented by its minimum corner index. */
            typedef Index Voxel;

            /** A directed voxel edge is represented by a ordered pair of voxels. */
            typedef std::pair<Voxel, Voxel> VoxelEdge;

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

                size_t size() const
                {
                    return _props.size();
                }

                void clear()
                {
                    _props.clear();
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

            /** Hasher for Voxel */
            struct HashVoxel {
                inline size_t operator()(const Voxel &v) const
                {
                    size_t seed = 0;
                    hashCombine(seed, v.x());
                    hashCombine(seed, v.y());
                    hashCombine(seed, v.z());
                    return seed;
                }
            };

            /** Equality for Voxel */
            struct EqualVoxel {
                inline bool operator()(const Voxel &a, const Voxel &b) const
                {
                    return a.x() == b.x() &&
                        a.y() == b.y() &&
                        a.z() == b.z();
                }
            };

            /** Sparse property assigned to voxels. */
            template<class Value>
            class SparseVoxelProperty : public SparseProperty<Voxel, Value, HashVoxel, EqualVoxel> 
            {
            public:
                SparseVoxelProperty(const Value &defaultValue)
                    : SparseProperty<Voxel, Value, HashVoxel, EqualVoxel>(defaultValue)
                {}
            };

            /** Hasher for VoxelEdge */
            struct HashVoxelEdge {
                inline size_t operator()(const VoxelEdge &v) const
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

            /** Equality for VoxelEdge */
            struct EqualVoxelEdge {
                inline bool operator()(const VoxelEdge &a, const VoxelEdge &b) const
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
            class SparseVoxelEdgeProperty : public SparseProperty<VoxelEdge, Value, HashVoxelEdge, EqualVoxelEdge> 
            {
            public:
                SparseVoxelEdgeProperty(const Value &defaultValue = Value())
                    : SparseProperty<VoxelEdge, Value, HashVoxelEdge, EqualVoxelEdge>(defaultValue)
                {}
            };

            /** Build a transformation that maps from world coordinates to voxel grid coordinates. */
            inline AffineTransform buildWorldToLocal(const Vector &origin, const Vector &resolution)
            {
                AffineTransform a;
                a.setIdentity();
                a.scale(resolution.cwiseInverse()).translate(-origin);
                return a;
            }

            /** Transform world point to voxel grid coordinates. */
            inline Vector worldToLocal(const AffineTransform &wl, const Vector &x)
            {
                return wl * x;
            }

            /** Transform world point to voxel it falls into. */
            inline Voxel worldToVoxel(const AffineTransform &wl, const Vector &x)
            {
                Vector l = worldToLocal(wl, x);
                return Voxel((int)floor(l.x()),
                    (int)floor(l.y()),
                    (int)floor(l.z()));
            }

            /** Returns the pair edge oriented in the opposite direction to given edge */
            inline VoxelEdge oppositeEdge(const VoxelEdge &e)
            {
                return VoxelEdge(e.second, e.first);
            }

            /** Returns the undirected version of an edge. This is defined as the positively
            oriented edge. */
            inline VoxelEdge undirectedEdge(const VoxelEdge &e)
            {
                // Undirected version of an edge always flows in the positive direction.
                if ((e.second - e.first).minCoeff() < 0)
                    return oppositeEdge(e);
                else
                    return e;
            }

            /** Voxel edges */
            template<class OutputIterator>
            inline void edges(OutputIterator iter)
            {
                // x+ edges
                *iter++ = VoxelEdge(Voxel(0,0,0), Voxel(1,0,0)); 
                *iter++ = VoxelEdge(Voxel(0,1,0), Voxel(1,1,0)); 
                *iter++ = VoxelEdge(Voxel(0,0,1), Voxel(1,0,1)); 
                *iter++ = VoxelEdge(Voxel(0,1,1), Voxel(1,1,1)); 

                // y+ edges
                *iter++ = VoxelEdge(Voxel(0,0,0), Voxel(0,1,0)); 
                *iter++ = VoxelEdge(Voxel(1,0,0), Voxel(1,1,0)); 
                *iter++ = VoxelEdge(Voxel(0,0,1), Voxel(0,1,1)); 
                *iter++ = VoxelEdge(Voxel(1,0,1), Voxel(1,1,1)); 

                // z+ edges
                *iter++ = VoxelEdge(Voxel(0,0,0), Voxel(0,0,1)); 
                *iter++ = VoxelEdge(Voxel(1,0,0), Voxel(1,0,1)); 
                *iter++ = VoxelEdge(Voxel(0,1,0), Voxel(0,1,1)); 
                *iter++ = VoxelEdge(Voxel(1,1,0), Voxel(1,1,1)); 

            }

            /** Visit all voxel edges for a single voxel. */
            template<class OutputIterator>
            inline void edges(const Voxel &v, OutputIterator iter)
            {
                VoxelEdge canonical_edges[12];
                edges(canonical_edges);

                for (size_t i = 0; i < 12; ++i) {
                    canonical_edges[i].first += v;
                    canonical_edges[i].second += v;
                    *iter++ = canonical_edges[i];
                }
            }

            /** Visit all voxel edges in the range spanned by [lower, upper). */
            template<class OutputIterator>
            inline void edges(const Voxel &lower, const Voxel &upper, OutputIterator iter)
            {
                // For each voxel visited, this reports the +x, +y, +z edge then moves to the
                // next voxel. Upon reaching upper, not all edges are reported.

                for (Voxel::Scalar z = lower.z(); z <= upper.z(); ++z) {
                    for (Voxel::Scalar y = lower.y(); y <= upper.y(); ++y) {
                        for (Voxel::Scalar x = lower.x(); x <= upper.x(); ++x) {
                            if (x < upper.x())
                                *iter++ = VoxelEdge(Voxel(x, y, z), Voxel(x + 1, y, z));
                            if (y < upper.y())
                                *iter++ = VoxelEdge(Voxel(x, y, z), Voxel(x, y + 1, z));
                            if (z < upper.z())
                                *iter++ = VoxelEdge(Voxel(x, y, z), Voxel(x, y, z + 1));
                        }
                    }
                }
            }

            /** Visit the four voxels sharing the given edge in counter-clockwise order. */
            template<class OutputIterator>
            inline void voxels(const VoxelEdge &e, OutputIterator iter)
            {                
                static Voxel::Index mod3[5] = {0, 1, 2, 0, 1};

                Voxel::Index i;                
                Voxel v;
                v.setZero();

                if ((e.second - e.first).maxCoeff(&i) > 0) {
                    *iter++ = e.first; 

                    v(mod3[i+1]) = 1; v(mod3[i+2]) = 0;
                    *iter++ = (e.first - v);

                    v(mod3[i+1]) = 1; v(mod3[i+2]) = 1;
                    *iter++ = (e.first - v);

                    v(mod3[i+1]) = 0; v(mod3[i+2]) = 1;
                    *iter++ = (e.first - v);

                } else if ((e.second - e.first).minCoeff(&i) < 0) {
                    const VoxelEdge eu = oppositeEdge(e);

                    v(mod3[i+1]) = 0; v(mod3[i+2]) = 1;
                    *iter++ = (eu.first - v);    

                    v(mod3[i+1]) = 1; v(mod3[i+2]) = 1;
                    *iter++ = (eu.first - v);

                    v(mod3[i+1]) = 1; v(mod3[i+2]) = 0;
                    *iter++ = (eu.first - v);

                    *iter++ = eu.first; 
                }
            }


        }
    }
}

#endif
