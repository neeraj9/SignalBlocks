// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// This file is part of SignalBlocks.
//
// SignalBlocks is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SignalBlocks is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SIGBLOCKS_FILESOURCECREATOR_H
#define SIGBLOCKS_FILESOURCECREATOR_H

#include "../common/IPort.h"
#include "../common/TimeTick.h"

#include <fstream>
#include <memory>
#include <string>

namespace signalblocks {
    template<template<class> class B, class T>
    class FileSourceCreator {
    public:
        static B<T>* Create(
                const std::string& filename,
                int blockSize) {
            B<T>* pStream_source = new B<T>("source-block", blockSize);
            std::unique_ptr<std::istream> pIns(new std::ifstream(
                    filename.c_str(), std::ifstream::in | std::ifstream::binary));
            pStream_source->SetStreamSource(std::move(pIns));
            return pStream_source;
        }
    };
}

#endif // SIGBLOCKS_FILESOURCECREATOR_H
