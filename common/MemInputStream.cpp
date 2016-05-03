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

#include "MemInputStream.h"

#include <assert.h>

using namespace sigblocks;

MemInputStream::MemInputStream(char* data, std::size_t len)
        : mpData(data),
          mLen(len) {
    setg(data, data, data + len);
    setp(data, data + len);
}

std::streambuf::pos_type
MemInputStream::seekoff(off_type off,
                        std::ios_base::seekdir dir,
                        std::ios_base::openmode which) {
    if (dir == std::ios_base::cur) {
        gbump(off);
    } else if (dir == std::ios_base::beg) {
        setg(mpData, mpData + off, mpData + mLen);
        // this->_M_in_cur = this->_M_in_beg + off;
    } else if (dir == std::ios_base::end) {
        setg(mpData, mpData + mLen + off, mpData + mLen);
        // this->_M_in_cur = this->_M_in_end + off;
    } else {
        assert(0);  // not possible
    }

    // return the offset of current location from the beginning
    return gptr() - eback();
}
