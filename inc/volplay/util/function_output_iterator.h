// (C) Copyright Jeremy Siek 2001.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// Revision History:
// 27 Feb 2001   Jeremy Siek
//      Initial checkin.

#ifndef VOLPLAY_UTIL_FUNCTION_OUTPUT_ITERATOR
#define VOLPLAY_UTIL_FUNCTION_OUTPUT_ITERATOR

namespace volplay {
    namespace util {

        /** Use any function as output iterator. Note this code is from Boost as noted
            in the above copy right notice. The only changes being made is coding style. */
        template <class UnaryFunction>
        class FunctionOutputIterator {
            typedef FunctionOutputIterator self;
        public:
            typedef std::output_iterator_tag iterator_category;
            typedef void                value_type;
            typedef void                difference_type;
            typedef void                pointer;
            typedef void                reference;

            explicit FunctionOutputIterator() {}

            explicit FunctionOutputIterator(const UnaryFunction& f)
                : m_f(f) {}

            struct OutputProxy {
                OutputProxy(UnaryFunction& f) : m_f(f) { }
                template <class T> OutputProxy& operator=(const T& value) {
                    m_f(value); 
                    return *this; 
                }
                UnaryFunction& m_f;
            };
            OutputProxy operator*() { return OutputProxy(m_f); }
            self& operator++() { return *this; } 
            self& operator++(int) { return *this; }
        private:
            UnaryFunction m_f;
        };

        /** Construct FunctionOutputIterator from function instance. */
        template <class UnaryFunction>
        inline FunctionOutputIterator<UnaryFunction>
        makeFunctionOutputIterator(const UnaryFunction& f = UnaryFunction()) {
                return FunctionOutputIterator<UnaryFunction>(f);
        }

        /** Construct FunctionOutputIterator from function instance. */
        template <class UnaryFunction>
        inline FunctionOutputIterator<UnaryFunction>
        oiter(const UnaryFunction& f = UnaryFunction()) {
                return FunctionOutputIterator<UnaryFunction>(f);
        }

    }
}

#endif