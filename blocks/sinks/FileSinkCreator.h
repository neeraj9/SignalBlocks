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

#ifndef SIGBLOCKS_FILESINKCREATOR_H
#define SIGBLOCKS_FILESINKCREATOR_H

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
            B<T>* pStream_sink = new B<T>("sink-block");
            std::unique_ptr<std::ostream> pOuts(new std::ofstream(
                    filename.c_str(), std::ofstream::out | std::ofstream::binary));
            pStream_sink->SetStreamSink(std::move(pOuts));
            return pStream_sink;
        }
    };
}

#endif // SIGBLOCKS_FILESINKCREATOR_H
