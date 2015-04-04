// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#ifndef VOLPLAY_MATH_ROOT
#define VOLPLAY_MATH_ROOT

#include <volplay/types.h>
#include <limits>

namespace volplay {
    namespace math {
        
        namespace detail {
            /** Helper function to make a same sign as b. */
            inline S makeSameSign(Scalar a, Scalar b) {
                return b >= 0 ? std::abs(a) : -std::abs(a);
            }
        }
        
        /** 
            Find root of function in interval.
            
            Uses Brent's method in the given interval. Implementation taken from
            Numerical Recipes in C.
        */
        template<class F>
        bool findRootBrent(F &f, Scalar x1, Scalar x2, Scalar tol, int maxIter, Scalar &root) {
            const Scalar eps = std::numeric_limits<Scalar>::epsilon();
            
            int iter;
            Scalar a=x1,b=x2,c=x2,d,e,min1,min2;
            Scalar fa=f(a),fb=f(b),fc,p,q,r,s,tol1,xm;
            
            if ((fa > 0 && fb > 0) || (fa < 0 && fb < 0))
                return false; // Root must be bracketed
            
            fc=fb;
            for (iter=1;iter<=maxIter;iter++) {
                if ((fb > 0 && fc > 0) || (fb < 0 && fc < 0)) {
                    c=a;
                    fc=fa;
                    e=d=b-a;
                }
                if (std::abs(fc) < std::abs(fb)) {
                    a=b;
                    b=c;
                    c=a;
                    fa=fb;
                    fb=fc;
                    fc=fa;
                }
                tol1=S(2)*eps*std::abs(b)+S(0.5)*tol;
                xm=S(0.5)*(c-b);
                if (std::abs(xm) <= tol1 || fb == 0.0) {
                    root = b;
                    return true;
                }
                
                if (std::abs(e) >= tol1 && std::abs(fa) > std::abs(fb)) {
                    // Attempt inverse quadratic interpolation.
                    s=fb/fa;
                    if (a == c) {
                        p=S(2.0)*xm*s;
                        q=S(1.0)-s;
                    } else {
                        q=fa/fc;
                        r=fb/fc;
                        p=s*(S(2.0)*xm*q*(q-r)-(b-a)*(r-S(1.0)));
                        q=(q-S(1.0))*(r-S(1.0))*(s-S(1.0));
                    }
                    if (p > 0.0)
                        q = -q; // Check whether in bounds.
                    
                    p=fabs(p);
                    min1=S(3.0)*xm*q-std::abs(tol1*q);
                    min2=std::abs(e*q);
                    
                    if (S(2.0)*p < (min1 < min2 ? min1 : min2)) {
                        e=d; // Accept interpolation.
                        d=p/q;
                    } else {
                        d=xm; // Interpolation failed, use bisection.
                        e=d;
                    }
                } else {
                    // Bounds decreasing too slowly, use bisection.
                    d=xm;
                    e=d;
                }
                a=b; // Move last best guess to a.
                fa=fb;
                if (std::abs(d) > tol1) // Evaluate new trial root.
                    b += d;
                else
                    b += detail::makeSameSign(tol1,xm);
                fb = f(b);
            }
            root = b;
            return false; // Max number of iterations exceeded.
        }
        
    }
}

#endif
