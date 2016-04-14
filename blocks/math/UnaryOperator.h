// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_UNARYOPERATOR_H
#define SIGBLOCKS_UNARYOPERATOR_H

#include "../../common/Port.h"

namespace sigblocks {
    template<class T>
    class UnaryOperator
            : public Port<1, 1, T> {
    protected:
        virtual T Compute(const T& arg1) const = 0;

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX change to an assertion library.
            T output = this->Compute(data);
            this->LeakData(0, output, startTime);
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX change to an assertion library.
            for (int i = 0; i < len; ++i) {
                T output = this->Compute(data.get()[i]);
                // use swap instead of assignment operator to avoid copy for complex data type T,
                // but the user defined data type T should define swap() method to get the benefit.
                std::swap(data.get()[i], output);
            }
            this->LeakData(0, std::move(data), len, startTime);

        }

        virtual void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX change to an assertion library.
            assert(! dims.empty());
            int len = dims[0];
            for (size_t i = 1; i < dims.size(); ++i) {
                len *= dims[i];
            }
            assert(len > 0);
            for (int i = 0; i < len; ++i) {
                T output = this->Compute(data.get()[i]);
                // use swap instead of assignment operator to avoid copy for complex data type T,
                // but the user defined data type T should define swap() method to get the benefit.
                std::swap(data.get()[i], output);
            }
            this->LeakMatrix(0, std::move(data), dims, startTime);
        }
    };
}

#endif //SIGBLOCKS_UNARYOPERATOR_H
