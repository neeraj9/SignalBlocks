// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_TAN_H
#define SIGBLOCKS_TAN_H

#include "UnaryOperator.h"

#include <cmath>

namespace sigblocks {
    // computes  \tan(x)
    template<class T>
    class Tan
            : public UnaryOperator<T> {
    protected: // UnaryOperator interface
        virtual T Compute(const T& arg1) const {
            return tan(arg1);
        }

    };
}

#endif // SIGBLOCKS_TAN_H
