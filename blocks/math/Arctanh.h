// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_ARCTANH_H
#define SIGBLOCKS_ARCTANH_H

#include "BinaryOperator.h"

#include <gsl/gsl_math.h>

namespace sigblocks {
    // computes  \arctanh(x)
    template<class T>
    class Arctanh
            : public BinaryOperator<T> {
    protected: // BinaryOperator interface
        virtual T Compute(const T& arg1) const {
            return gsl_atanh(arg1);
        }

    };
}

#endif // SIGBLOCKS_ARCTANH_H
