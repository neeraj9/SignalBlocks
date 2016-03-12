// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_Tan_hh
#define sigblocks_math_Tan_hh

#include "BinaryOperator.hh"

#include <cmath>

namespace sigblocks {
    // computes  \tan(x)
    template<class T>
    class Tan
            : public BinaryOperator<T> {
    protected: // BinaryOperator interface
        virtual T Compute(const T& arg1) const {
            return tan(arg1);
        }

    };
}

#endif // sigblocks_math_Tan_hh
