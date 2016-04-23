// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#include "MemInputStream.h"

using namespace sigblocks;

MemInputStream::MemInputStream(char* data, std::size_t len)
        : mpData(data),
          mLen(len) {
    setg(data, data, data + len);
    setp(data, data + len);
}

std::streambuf::pos_type
MemInputStream::seekoff(long off,
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
