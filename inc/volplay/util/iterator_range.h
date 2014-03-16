// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_ITERATOR_RANGE
#define VOLPLAY_ITERATOR_RANGE

namespace volplay {
    namespace util {
        
        /** Helper class to use range based for loops with iterator pairs. */
        template <typename Iterator>
        class IteratorRange {
        public:
            IteratorRange ( Iterator first, Iterator last )
            : _first(first), _last(last)
            {}
            
            Iterator begin () const { return _first; }
            Iterator end   () const { return _last; }
            
        private:
            Iterator _first;
            Iterator _last;
        };
        
        /** Helper method to create IteratorRange from two iterators. */
        template <typename Iterator>
        IteratorRange<Iterator> makeIteratorRange (Iterator first, Iterator last)
        {
            return IteratorRange<Iterator>(first, last);
        }
    }
}

#endif