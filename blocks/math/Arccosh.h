// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_ARCCOSH_H
#define SIGBLOCKS_ARCCOSH_H

#include "UnaryOperator.h"

#include <gsl/gsl_math.h>

namespace sigblocks {
    // computes  \arccosh(x)
    template<class T>
    class Arccosh
            : public UnaryOperator<T> {
    protected: // UnaryOperator interface
        virtual T Compute(const T& arg1) const {
            return gsl_acosh(arg1);
        }

    };
}

#endif // SIGBLOCKS_ARCCOSH_H
