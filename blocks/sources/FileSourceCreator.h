// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_FILESOURCECREATOR_H
#define SIGBLOCKS_FILESOURCECREATOR_H

#include "../common/IPort.h"
#include "../common/TimeTick.h"

#include <fstream>
#include <memory>
#include <string>

namespace sigblocks {
    template<template<class> class B, class T>
    class FileSourceCreator {
    public:
        static B<T>* Create(
                const std::string& filename,
                int blockSize) {
            B<T>* pStream_source = new B<T>(blockSize);
            std::unique_ptr<std::istream> pIns(new std::ifstream(
                    filename.c_str(), std::ifstream::in | std::ifstream::binary));
            pStream_source->SetStreamSource(std::move(pIns));
            return pStream_source;
        }
    };
}

#endif // SIGBLOCKS_FILESOURCECREATOR_H
