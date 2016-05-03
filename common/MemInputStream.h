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

#ifndef SIGNALBLOCKS_MEMINPUTSTREAM_H
#define SIGNALBLOCKS_MEMINPUTSTREAM_H

#include <streambuf>

namespace sigblocks {

    class MemInputStream : public std::streambuf {
    public:
        MemInputStream(char* data, std::size_t len);

        // Override virtual methods of std::streambuf so that
        // std::istream(this).seekg(off, dir) and
        // std::istream(this).tellg() are implemented correctly.
        // See std::istream source for seekg(off, dir) and tellg()
        // implementations and notice that eventually
        // std::streambuf::seekoff() is invoked, which is not
        // implemented and must be overridden here.
        virtual std::streambuf::pos_type seekoff(
                off_type off,
                std::ios_base::seekdir dir,
                std::ios_base::openmode which = std::ios_base::in);

    private:
        char* mpData;
        std::size_t mLen;
    };
}

#endif //SIGNALBLOCKS_MEMINPUTSTREAM_H
