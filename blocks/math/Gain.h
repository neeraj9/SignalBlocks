// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_GAIN_H
#define SIGBLOCKS_GAIN_H

#include "../../common/Port.h"

namespace sigblocks {
    template<class T>
    class Gain
            : public Port<1, 1, T> {
    public:
        Gain(const T& scale)
                : mScale(scale) {
        }

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            this->LeakData(0, data * mScale, startTime);
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            for (int i = 0; i < len; ++i) {
                data.get()[i] *= mScale;
            }
            this->LeakData(0, std::move(data), len, startTime);
        }

        virtual void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(! dims.empty());
            int len = dims[0];
            for (size_t i = 1; i < dims.size(); ++i) {
                len *= dims[i];
            }
            for (int i = 0; i < len; ++i) {
                data.get()[i] *= mScale;
            }
            this->LeakMatrix(0, std::move(data), dims, startTime);
        }

    private:
        const T mScale;
    };
}

#endif // SIGBLOCKS_GAIN_H
