// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "OstreamSink.h"

#include <fstream>
#include <iostream>

using namespace sigblocks;
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
