// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_common_signalprocessing_Conv_hh
#define sigblocks_common_signalprocessing_Conv_hh

namespace sigblocks {
    template<class T>
    class Conv {
    public:
        Compute(const T& arg1, const T& arg2, T* output);
    };
}

#endif // sigblocks_common_signalprocessing_Conv_hh