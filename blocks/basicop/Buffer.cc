// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include <assert.h>
#include "Buffer.hh"

#include <algorithm> // std::copy

using namespace sigblocks;
using namespace std;

template <class T>
Buffer<T>::Buffer(int fieldsToBuffer)
  : mFieldsToBuffer(fieldsToBuffer),
    mNumFields(0),
    mData(new T[fieldsToBuffer]) // pre-allocate space
{
}

template <class T>
Buffer<T>::~Buffer()
{
}

template <class T>
void
Buffer<T>::Process(int sourceIndex, const T& data, const TimeTick& startTime)
{
  assert(sourceIndex == 0); // XXX change to an assertion library.

  mData.get()[mNumFields++] = data;
  if (mNumFields >= mFieldsToBuffer)
  {
    this->LeakData(0, std::move(mData), mNumFields, startTime);
    if (mData.get() == 0) // this is always true at present
    {
      mData.reset(new T[mFieldsToBuffer]);
    }
    mNumFields = 0;
  }
}

// Note: the source of this module can call any combination of the above
// or this method and the buffering will work. This is done to allow the
// additional flexibility to the user to mix both interfaces (scalar and vector).
template <class T>
void
Buffer<T>::Process(
  int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime)
{
  assert(sourceIndex == 0); // XXX change to an assertion library.

  int offset = 0;
  int fields_to_copy = ((mNumFields + len - mFieldsToBuffer) >= 0) ? (mFieldsToBuffer - mNumFields) : len;
  while (fields_to_copy > 0)
  {
    // fill mData with rest of the contents
    std::copy(data.get() + offset,
              data.get() + fields_to_copy,
              mData.get() + mNumFields);
    mNumFields += fields_to_copy;
    if (mNumFields >= mFieldsToBuffer)
    {
      this->LeakData(0, std::move(mData), mNumFields, startTime);
      if (mData.get() == 0) // this is always true at present
      {
        mData.reset(new T[mFieldsToBuffer]);
      }
      mNumFields = 0;
    }
    offset += fields_to_copy;
    fields_to_copy = ((mNumFields + len - offset - mFieldsToBuffer) >= 0) ? (mFieldsToBuffer - mNumFields) : (len - offset);
  }
}

template class Buffer<long>;
template class Buffer<unsigned long>;
template class Buffer<int>;
template class Buffer<unsigned int>;
template class Buffer<unsigned char>;
template class Buffer<float>;
template class Buffer<double>;
