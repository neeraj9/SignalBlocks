// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "ComplexStreamSource.hh"

#include <fstream>
#include <iostream>

using namespace sigblocks;
using namespace std;

template<class T>
ComplexStreamSource<T>::ComplexStreamSource(
        const TimeTick& startTime, TimeTick& increment,
        int blockSize)
        : mTime(startTime),
          mIncrement(increment),
          mpComplexStream(nullptr),
          mLoopOver(true),
          mBlockSize(blockSize) {
}

template<class T>
ComplexStreamSource<T>::ComplexStreamSource(
        const TimeTick& startTime, TimeTick& increment,
        std::unique_ptr<std::istream> pIns, int blockSize)
        : mTime(startTime),
          mIncrement(increment),
          mpComplexStream(std::move(pIns)),
          mLoopOver(true),
          mBlockSize(blockSize) {
}

template<class T>
void
ComplexStreamSource<T>::SetStreamSource(std::unique_ptr<std::istream> pIns) {
    mpComplexStream.swap(pIns);
}

template<class T>
void
ComplexStreamSource<T>::Loop(bool loopOver) {
    mLoopOver = loopOver;
}

template<class T>
void
ComplexStreamSource<T>::Generate() {
    if (mpComplexStream->eof() && !mLoopOver) {
        cerr << this << " ComplexStreamSource is done processing!" << endl;
        return; // dont do anything
    }

    std::unique_ptr<T[]> data(new T[2 * mBlockSize]);
    // There are two values per sample, RF Sample = {I,Q}.
    mpComplexStream->read(reinterpret_cast<char*>(data.get()), mBlockSize * 2 * sizeof(T));
    int bytes_read = mpComplexStream->gcount() / sizeof(char);
    this->LeakData(0, std::move(data), bytes_read, mTime);
    mTime += mIncrement;
    if (mpComplexStream->eof() && mLoopOver) {
        mpComplexStream->clear();
        mpComplexStream->seekg(0, std::ios::beg);
    }
}

template
class ComplexStreamSource<short>;
