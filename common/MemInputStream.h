// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

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
