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

#include "OstreamSink.h"

#include <fstream>
#include <iostream>

using namespace signalblocks;
using namespace std;

#define DESCRIPTION "An output stream sink, where the incomig data passes through to the stream."

template<class T>
OstreamSink<T>::OstreamSink(std::string name)
        : Port<1, 0, T>(std::move(name), DESCRIPTION),
          mpOstream(nullptr) {
}

template<class T>
OstreamSink<T>::OstreamSink(std::string name, std::unique_ptr<std::ostream> pOuts)
        : Port<1, 0, T>(std::move(name), DESCRIPTION),
          mpOstream(std::move(pOuts)) {
}

template<class T>
void
OstreamSink<T>::SetStreamSink(std::unique_ptr<std::ostream> pOuts) {
    mpOstream.swap(pOuts);
}

template<class T>
void
OstreamSink<T>::Process(int sourceIndex, const T& data, const TimeTick& startTime) {
    mpOstream->write(reinterpret_cast<const char*>(&data), sizeof(T));
}

template<class T>
void
OstreamSink<T>::Process(
        int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
    mpOstream->write(reinterpret_cast<const char*>(data.get()), len * sizeof(T));
}

namespace signalblocks {

    template
    class OstreamSink<unsigned char>;

    template
    class OstreamSink<char>;

    template
    class OstreamSink<short>;

    template
    class OstreamSink<int>;

    template
    class OstreamSink<long>;

}