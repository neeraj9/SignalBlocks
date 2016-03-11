// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "IstreamSource.hh"

#include <fstream>
#include <iostream>

using namespace sigblocks;
using namespace std;

template<class T>
IstreamSource<T>::IstreamSource(
        const TimeTick& startTime, TimeTick& increment,
        int blockSize)
        : mTime(startTime),
          mIncrement(increment),
          mpIstream(nullptr),
          mLoopOver(true),
          mBlockSize(blockSize) {
}

template<class T>
IstreamSource<T>::IstreamSource(
        const TimeTick& startTime, TimeTick& increment,
        std::unique_ptr<std::istream> pIns, int blockSize)
        : mTime(startTime),
          mIncrement(increment),
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
IstreamSource<T>::Generate() {
    if (mpIstream->eof() && !mLoopOver) {
        cerr << this << " IstreamSource is done processing!" << endl;
        return; // dont do anything
    }

    std::unique_ptr<T[]> data(new T[mBlockSize]);
    mpIstream->read(reinterpret_cast<char*>(data.get()), mBlockSize * sizeof(T));
    int bytes_read = mpIstream->gcount() / sizeof(char);
    this->LeakData(0, std::move(data), bytes_read, mTime);
    mTime += mIncrement;
    if (mpIstream->eof() && mLoopOver) {
        mpIstream->clear();
        mpIstream->seekg(0, std::ios::beg);
    }
}

template
class IstreamSource<unsigned char>;
