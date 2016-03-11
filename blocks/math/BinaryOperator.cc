// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include <assert.h>
#include "BinaryOperator.hh"

using namespace sigblocks;
using namespace std;

template <class T>
void
BinaryOperator<T>::Process(int sourceIndex, const T& data, const TimeTick& startTime)
{
  assert(sourceIndex == 0 || sourceIndex == 1);
  mDataPort[sourceIndex].push_back(data);
  mDataPortTime[sourceIndex].push_back(startTime);
  size_t min_elements = mDataPort[0].size();
  if (min_elements > mDataPort[1].size())
  {
    min_elements = mDataPort[1].size();
  }
  if (min_elements > 0)
  {
    list<T> result;
    list<TimeTick> timelist;
    if (mDataPortTime[0].front() == mDataPortTime[1].front())
    {
      result.push_back(Compute(mDataPort[0].front(), mDataPort[1].front()));
      timelist.push_back(mDataPortTime[0].front());
    }
    else if (mDataPortTime[0].front() < mDataPortTime[1].front())
    {
      result.push_back(mDataPort[0].front());
      result.push_back(mDataPort[1].front());
      timelist.push_back(mDataPortTime[0].front());
      timelist.push_back(mDataPortTime[1].front());
    }
    else
    {
      result.push_back(mDataPort[1].front());
      result.push_back(mDataPort[0].front());
      timelist.push_back(mDataPortTime[1].front());
      timelist.push_back(mDataPortTime[0].front());
    }
    mDataPort[0].pop_front();
    mDataPort[1].pop_front();
    typename list<T>::iterator iter;
    iter = result.begin();
    for (; iter != result.end(); ++iter)
    {
      this->LeakData(0, *iter, timelist.front());
      timelist.pop_front();
    }
  }
}

template <class T>
void
BinaryOperator<T>::Process(
  int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime)
{
  assert(0); // FIXME
}

template class BinaryOperator<long>;
template class BinaryOperator<unsigned long>;
template class BinaryOperator<int>;
template class BinaryOperator<unsigned int>;
template class BinaryOperator<short>;
template class BinaryOperator<unsigned short>;
template class BinaryOperator<char>;
template class BinaryOperator<unsigned char>;
template class BinaryOperator<float>;
template class BinaryOperator<double>;
