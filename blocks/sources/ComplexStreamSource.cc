// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "ComplexStreamSource.hh"

#include "../../common/MultiPtr.hh"

#include <fstream>
#include <iostream>

using namespace iblocks;
using namespace std;

template<class T>
ComplexStreamSource<T>::ComplexStreamSource(
  const TimeTick& startTime, TimeTick& increment,
  int blockSize)
  : mTime(startTime),
    mIncrement(increment),
    mpComplexStream(0),
    mLoopOver(true),
    mBlockSize(blockSize)
{
}

template<class T>
ComplexStreamSource<T>::ComplexStreamSource(
  const TimeTick& startTime, TimeTick& increment,
  std::auto_ptr<std::istream> pIns, int blockSize)
  : mTime(startTime),
    mIncrement(increment),
    mpComplexStream(pIns),
    mLoopOver(true),
    mBlockSize(blockSize)
{
}

template<class T>
void
ComplexStreamSource<T>::SetStreamSource(std::auto_ptr<std::istream> pIns)
{
  mpComplexStream = pIns;
}

template<class T>
void
ComplexStreamSource<T>::Loop(bool loopOver)
{
  mLoopOver = loopOver;
}

template<class T>
void
ComplexStreamSource<T>::Generate()
{
  if (mpComplexStream->eof() && !mLoopOver)
  {
    cerr << this << " ComplexStreamSource is done processing!" << endl;
    return; // dont do anything
  }

  MultiPtr<T> pData(new T[2 * mBlockSize]);
  // There are two values per sample, RF Sample = {I,Q}.
  mpComplexStream->read(reinterpret_cast<char*>(pData.get()), mBlockSize * 2 * sizeof(T));
  int bytes_read = mpComplexStream->gcount() / sizeof(char);
  this->LeakData(0, pData, bytes_read, mTime);
  mTime += mIncrement;
  if (mpComplexStream->eof() && mLoopOver)
  {
    mpComplexStream->clear();
    mpComplexStream->seekg(0, std::ios::beg);
  }
}

template class ComplexStreamSource<short>;
