// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_COS_H
#define SIGBLOCKS_COS_H

#include "UnaryOperator.h"

#include <cmath>

namespace sigblocks {
    // computes  \cos(x)
    template<class T>
    class Cos
            : public UnaryOperator<T> {
    protected: // UnaryOperator interface
        virtual T Compute(const T& arg1) const {
            return cos(arg1);
        }

    };
}

#endif // SIGBLOCKS_COS_H
