// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_PRODUCT_H
#define SIGBLOCKS_PRODUCT_H

#include "NOperator.h"

namespace sigblocks {
    template<int N, class T>
    class Product
            : public NOperator<N, T> {
    protected: // NOperator interface

        virtual void Compute(T* pArgs[N], const std::vector<int>& dims) {
            T result = *pArgs[0];
            for (int i = 1; i < N; ++i) {
                result *= (*pArgs[i]);  // TODO handle saturate
            }
            *pArgs[0] = result;
        }
    };
}

#endif // SIGBLOCKS_PRODUCT_H
