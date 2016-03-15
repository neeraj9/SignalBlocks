// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_SUM_H
#define SIGBLOCKS_SUM_H

#include "NOperator.h"

namespace sigblocks {
    template<int N, class T>
    class Sum
            : public NOperator<N, T> {
    protected: // NOperator interface

        virtual void Compute(T* pArgs[N], const std::vector<int>& dims) {
            int len = dims[0];
            for (size_t i = 1; i < dims.size(); ++i) {
                len *= dims[i];
            }
            for (int i = 0; i < len; ++i) {
                T result = pArgs[0][i];
                for (int j = 1; j < N; ++j) {
                    result += pArgs[j][i];  // TODO handle saturate
                }
                pArgs[0][i] = result;
            }
        }
    };
}

#endif // SIGBLOCKS_SUM_H
