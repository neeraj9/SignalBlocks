// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_blocks_sink_FileSinkCreator_hh
#define sigblocks_blocks_sink_FileSinkCreator_hh

#include "../common/IPort.h"

#include <fstream>
#include <memory>
#include <string>

namespace sigblocks {
    template<template<class> class B, class T>
    class FileSinkCreator {
    public:
        static B<T>* Create(
                const std::string& filename) {
            B<T>* pStream_sink = new B<T>();
            std::unique_ptr<std::ostream> pOuts(new std::ofstream(
                    filename.c_str(), std::ofstream::out | std::ofstream::binary));
            pStream_sink->SetStreamSink(std::move(pOuts));
            return pStream_sink;
        }
    };
}

#endif // sigblocks_blocks_sink_FileSinkCreator_hh
