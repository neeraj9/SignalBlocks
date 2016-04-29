// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_ARCTANH_H
#define SIGBLOCKS_ARCTANH_H

#include "UnaryOperator.h"

#include <gsl/gsl_math.h>

namespace sigblocks {
    // computes  \arctanh(x)
    template<class T>
    class Arctanh
            : public UnaryOperator<T> {
    public:
        Arctanh(std::string name)
                : UnaryOperator<T>(std::move(name)) {
        }

    protected: // UnaryOperator interface
        virtual T Compute(const T& arg1) const {
            return gsl_atanh(arg1);
        }

    };
}

#endif // SIGBLOCKS_ARCTANH_H
