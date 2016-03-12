// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "IstreamSource.hh"

#include <fstream>
#include <iostream>

using namespace sigblocks;
using namespace std;

template<class T>
IstreamSource<T>::IstreamSource(int blockSize)
        : mLastTick(),
          mpIstream(nullptr),
          mLoopOver(true),
          mBlockSize(blockSize) {
}

template<class T>
IstreamSource<T>::IstreamSource(
        int blockSize,
        std::unique_ptr<std::istream> pIns)
        : mLastTick(),
          mpIstream(std::move(pIns)),
          mLoopOver(true),
          mBlockSize(blockSize) {
}

template<class T>
void
IstreamSource<T>::SetStreamSource(std::unique_ptr<std::istream> pIns) {
    mpIstream.swap(pIns);
}

template<class T>
void
IstreamSource<T>::Loop(bool loopOver) {
    mLoopOver = loopOver;
}

template<class T>
void
IstreamSource<T>::ClockCycle(const TimeTick& timeTick) {
    if (mpIstream->eof() && !mLoopOver) {
        cerr << this << " IstreamSource is done processing!" << endl;
        return; // dont do anything
    }
    if (mLastTick == timeTick) {
        return;  // already processed the event
    }
    mLastTick = timeTick;

    std::unique_ptr<T[]> data(new T[mBlockSize]);
    mpIstream->read(reinterpret_cast<char*>(data.get()), mBlockSize * sizeof(T));
    int bytes_read = mpIstream->gcount() / sizeof(char);
    this->LeakData(0, std::move(data), bytes_read, timeTick);
    if (mpIstream->eof() && mLoopOver) {
        mpIstream->clear();
        mpIstream->seekg(0, std::ios::beg);
    }
}

template
class IstreamSource<unsigned char>;
