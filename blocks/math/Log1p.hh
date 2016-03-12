// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_Log1p_hh
#define sigblocks_math_Log1p_hh

#include "BinaryOperator.hh"

#include <gsl/gsl_math.h>

namespace sigblocks {
    // computes  \log(1+x)
    template<class T>
    class Log1p
            : public BinaryOperator<T> {
    protected: // BinaryOperator interface
        virtual T Compute(const T& arg1) const {
            return gsl_log1p(arg1);
        }

    };
}

#endif // sigblocks_math_Log1p_hh
