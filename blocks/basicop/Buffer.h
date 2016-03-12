// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_Buffer_hh
#define sigblocks_math_Buffer_hh

#include "../../common/Port.h"

#include <vector>

namespace sigblocks {
    template<class T>
    class Buffer
            : public Port<1, 1, T> {
    public:
        Buffer(int fieldsToBuffer);

        virtual ~Buffer();

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime);

    private:
        int mFieldsToBuffer; // avoid const so that it can be changed dynamically
        int mNumFields; // fields buffered so far
        std::unique_ptr<T[]> mData;
    };
}

#endif // sigblocks_math_Buffer_hh
