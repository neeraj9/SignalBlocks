// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_Cos_hh
#define sigblocks_math_Cos_hh

#include "BinaryOperator.h"

#include <cmath>

namespace sigblocks {
    // computes  \cos(x)
    template<class T>
    class Cos
            : public BinaryOperator<T> {
    protected: // BinaryOperator interface
        virtual T Compute(const T& arg1) const {
            return cos(arg1);
        }

    };
}

#endif // sigblocks_math_Cos_hh
