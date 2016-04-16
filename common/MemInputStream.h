// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#ifndef SIGNALBLOCKS_MEMINPUTSTREAM_H
#define SIGNALBLOCKS_MEMINPUTSTREAM_H

#include <streambuf>

namespace sigblocks {

    class MemInputStream : public std::streambuf {
    public:
        MemInputStream(char* data, std::size_t len);
    };
}

#endif //SIGNALBLOCKS_MEMINPUTSTREAM_H
