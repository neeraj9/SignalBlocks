// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_SIN_H
#define SIGBLOCKS_SIN_H

#include "UnaryOperator.h"

#include <cmath>

namespace sigblocks {
    // computes  \sin(x)
    template<class T>
    class Sin
            : public UnaryOperator<T> {
    public:
        Sin(std::string name)
                : UnaryOperator<T>(std::move(name)) {
        }

    protected: // UnaryOperator interface
        virtual T Compute(const T& arg1) const {
            return sin(arg1);
        }

    };
}

#endif // SIGBLOCKS_SIN_H
