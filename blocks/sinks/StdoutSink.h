// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// This file is part of SignalBlocks.
//
// SignalBlocks is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SignalBlocks is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SIGBLOCKS_STDOUTSINK_H
#define SIGBLOCKS_STDOUTSINK_H

#include "../../common/Port.h"

#include <iostream>

namespace signalblocks {
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
