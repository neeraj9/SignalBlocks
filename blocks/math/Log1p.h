// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_LOG1P_H
#define SIGBLOCKS_LOG1P_H

#include "UnaryOperator.h"

#include <gsl/gsl_math.h>

namespace sigblocks {
    // computes  \log(1+x)
    template<class T>
    class Log1p
            : public UnaryOperator<T> {
    public:
        Log1p(std::string name)
                : UnaryOperator<T>(std::move(name)) {
        }

    protected: // UnaryOperator interface
        virtual T Compute(const T& arg1) const {
            return gsl_log1p(arg1);
        }

    };
}

#endif // SIGBLOCKS_LOG1P_H
