// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_DIFFERENCE_H
#define SIGBLOCKS_DIFFERENCE_H

#include "NOperator.h"

namespace sigblocks {
    // implement a(n) - b(n) - c(n) - ...
    template<int N, class T>
    class Difference
            : public NOperator<N, T> {
    protected: // NOperator interface
        // args[0], ... args[N] = a(n), b(n), ...
        // implement args[0] - args[1] - ... args[N-1]
        virtual void Compute(T* pArgs[N], const std::vector<int>& dims) {
            int len = dims[0];
            for (size_t i = 1; i < dims.size(); ++i) {
                len *= dims[i];
            }
            for (int i = 0; i < len; ++i) {
                T result = pArgs[0][i];
                for (int j = 1; j < N; ++j) {
                    result -= pArgs[j][i];  // TODO handle underflow
                }
                pArgs[0][i] = result;
            }
        }
    };
}

#endif // SIGBLOCKS_DIFFERENCE_H
