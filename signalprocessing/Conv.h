// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_CONV_H
#define SIGBLOCKS_CONV_H

namespace sigblocks {
    template<class T>
    class Conv {
    public:
        Compute(const T& arg1, const T& arg2, T* output);
    };
}

#endif // SIGBLOCKS_SIGNALPROCESSING_CONV_H
