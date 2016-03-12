// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "PlotSink.h"

#include <iostream>
#include <assert.h>

using namespace sigblocks;
using namespace std;

template<class T> const int PlotSink<T>::MAX_SAMPLES;

template<class T>
PlotSink<T>::PlotSink()
#ifdef HAVE_PLPLOT
: mPlot("abc123"),
  mNumSamples(0)
#else
        : mNumSamples(0)
#endif
{
}

template<class T>
void
PlotSink<T>::Process(int sourceIndex, const T& data, const TimeTick& startTime) {
    assert(sourceIndex == 0);
    if (mNumSamples < MAX_SAMPLES) {
        // buffer
        mBuffer[mNumSamples++] = data;
    }
    else {
#ifdef HAVE_PLPLOT
        mPlot.Plot1D(&mBuffer[0], mNumSamples);
#endif
        mNumSamples = 0;
    }
#if 0
    cout << &dec << startTime.GetValue() << ": " <<
            &hex << data << &dec << endl;
            //&hex << static_cast<int>(data) << &dec << endl;
#endif
}

template<class T>
void
PlotSink<T>::Process(
        int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
    assert(sourceIndex == 0);
    cout << &dec << startTime.GetValue() << ": " << &hex;
    for (int i = 0; i < len; ++i) {
        cout << (data.get()[i]) << ", ";
        //cout << static_cast<unsigned long>(data.get()[i]) << ", ";
    }
    cout << &dec << endl;
}

// instantiate usuals
template
class PlotSink<unsigned char>;

template
class PlotSink<short>;

template
class PlotSink<int>;

template
class PlotSink<long>;

template
class PlotSink<float>;

template
class PlotSink<double>;
