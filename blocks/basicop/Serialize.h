// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_Serialize_hh
#define sigblocks_math_Serialize_hh

#include "../../common/Port.h"

namespace sigblocks {
    template<class T>
    class Serialize
            : public Port<1, 1, T> {
    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            LeakData(0, data, startTime);
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX change to an assertion library.
            for (int i = 0; i < len; ++i) {
                LeakData(0, data.get()[i], startTime);
            }
        }
    };
}

#endif // sigblocks_math_Serialize_hh
