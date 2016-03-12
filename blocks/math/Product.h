// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_Product_hh
#define sigblocks_math_Product_hh

#include "NOperator.h"

namespace sigblocks {
    template<int N, class T>
    class Product
            : public NOperator<N, T> {
    protected: // NOperator interface
        virtual T Compute(const T& arg1) const {
            return arg1;
        }

        virtual T Compute(const T& arg1, const T& arg2) const {
            // XXX saturate the output if enabled.
            return (arg1 * arg2);
        }

        virtual T Compute(const std::list<T>& args) const {
            typename std::list<T>::const_iterator iter = args.begin();
            T result = *iter; // at least one is present
            ++iter;
            for (; iter != args.end(); ++iter) {
                result = result * (*iter); // XXX saturate if enabled.
            }
            return result;
        }
    };
}

#endif // sigblocks_math_Product_hh
