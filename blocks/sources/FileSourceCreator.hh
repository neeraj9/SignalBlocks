// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_blocks_source_FileSourceCreator_hh
#define iblocks_blocks_source_FileSourceCreator_hh

#include "../common/IPort.hh"
#include "../common/TimeTick.hh"

#include <fstream>
#include <memory>
#include <string>

namespace iblocks
{
  template <template <class> class B, class T>
  class FileSourceCreator
  {
  public:
    static B<T>* Create(
      const TimeTick& startTime,
      TimeTick& increment,
      const std::string& filename,
      int blockSize)
    {
      B<T>* pStream_source = new B<T>(startTime, increment, blockSize);
      std::auto_ptr<std::istream> pIns(new std::ifstream(
        filename.c_str(), std::ifstream::in | std::ifstream::binary));
      pStream_source->SetStreamSource(pIns);
      return pStream_source;
    }
  };
}

#endif // iblocks_blocks_source_FileSourceCreator_hh
