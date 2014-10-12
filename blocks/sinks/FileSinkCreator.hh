// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef iblocks_blocks_sink_FileSinkCreator_hh
#define iblocks_blocks_sink_FileSinkCreator_hh

#include "../common/IPort.hh"

#include <fstream>
#include <memory>
#include <string>

namespace iblocks
{
  template <template <class> class B, class T>
  class FileSinkCreator
  {
  public:
    static B<T>* Create(
      const std::string& filename)
    {
      B<T>* pStream_sink = new B<T>();
      std::auto_ptr<std::ostream> pOuts(new std::ofstream(
        filename.c_str(), std::ofstream::out | std::ofstream::binary));
      pStream_sink->SetStreamSink(pOuts);
      return pStream_sink;
    }
  };
}

#endif // iblocks_blocks_sink_FileSinkCreator_hh
