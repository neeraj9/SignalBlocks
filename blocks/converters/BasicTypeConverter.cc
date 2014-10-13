// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "BasicTypeConverter.hh"

#include "../../common/GenericCopy.hh"

using namespace sigblocks;

template <class TFROM, class TTO>
void
BasicTypeConverter<TFROM,TTO>::Process(
  int sourceIndex,
  const TFROM& data,
  const TimeTick& startTime)
{
  assert(sourceIndex == 0);
  const TTO todata = static_cast<const TTO>(data);
  this->LeakData(0, todata, startTime);
}

template <class TFROM, class TTO>
void
BasicTypeConverter<TFROM,TTO>::Process(
  int sourceIndex, MultiPtr<TFROM> pData, int len, const TimeTick& startTime)
{
  assert(sourceIndex == 0);
  MultiPtr<TTO> todata(new TTO[len]);
  GenericCopy<TFROM, TTO>::Copy(pData.get(), pData.get() + len, todata.get());
  this->LeakData(0, todata, len, startTime);
}

template class BasicTypeConverter<int, float>;
