// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_STDOUTSINK_H
#define SIGBLOCKS_STDOUTSINK_H

#include "../../common/Port.h"

#include <iostream>

namespace sigblocks {
    template<class T>
    class StdoutSink
            : public Port<1, 0, T> {
    public:
        StdoutSink(std::string name)
                : Port<1, 0, T>(std::move(name), "A stdout sink."),
                  mPrefix() {
        }

        StdoutSink(std::string name,
                   const std::string& prefix)
                : Port<1, 0, T>(std::move(name), "A stdout sink."),
                  mPrefix(prefix) {
        }

    public: // Port interface
        void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            std::cout << mPrefix << &std::dec << startTime.GetValue() << ": " <<
            &std::hex << data << &std::dec << std::endl;
            //&std::hex << static_cast<int>(data) << &std::dec << std::endl;
        }

        void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            std::cout << mPrefix << &std::dec << startTime.GetValue() << ": " << &std::hex;
            for (int i = 0; i < len; ++i) {
                std::cout << (data.get()[i]) << ", ";
                //std::cout << static_cast<unsigned long>(data.get()[i]) << ", ";
            }
            std::cout << &std::dec << std::endl;
        }

    private:
        std::string mPrefix;
    };
}

#endif // SIGBLOCKS_STDOUTSINK_H
