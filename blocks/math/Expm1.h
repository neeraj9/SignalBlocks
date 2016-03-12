// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_EXPM1_H
#define SIGBLOCKS_EXPM1_H

#include "BinaryOperator.h"

#include <gsl/gsl_math.h>

namespace sigblocks {
    // computes  \exp(x)-1
    template<class T>
    class Expm1
            : public BinaryOperator<T> {
    protected: // BinaryOperator interface
        virtual T Compute(const T& arg1) const {
            return gsl_expm1(arg1);
        }

    };
}

#endif // SIGBLOCKS_EXPM1_H
