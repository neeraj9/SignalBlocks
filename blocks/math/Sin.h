// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_SIN_H
#define SIGBLOCKS_SIN_H

#include "BinaryOperator.h"

#include <cmath>

namespace sigblocks {
    // computes  \sin(x)
    template<class T>
    class Sin
            : public BinaryOperator<T> {
    protected: // BinaryOperator interface
        virtual T Compute(const T& arg1) const {
            return sin(arg1);
        }

    };
}

#endif // SIGBLOCKS_SIN_H
