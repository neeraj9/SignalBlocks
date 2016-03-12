// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_GENERICCOPY_H
#define SIGBLOCKS_GENERICCOPY_H

#include <algorithm>
#include <cmath>
#include <complex>
#include <string.h>  // memcpy
#include <typeinfo> 

namespace sigblocks {
    /**
     * Copy arbitrary data-types
     */
    template<class X, class Y>
    class GenericCopy {
    public:
        static void Copy(const X* srcStart, const X* srcEnd, Y* dest) {
            std::copy(srcStart, srcEnd, dest);
        }
    };

    template<class Y>
    class GenericCopy<std::complex<double>, Y> {
    public:
        static void Copy(
                const std::complex<double>* srcStart,
                const std::complex<double>* srcEnd,
                Y* dest) {
            if (typeid(Y) == typeid(std::complex<double>)) {
                memcpy(dest, srcStart, (srcEnd - srcStart));
            }
            else {
                for (const std::complex<double>* current = srcStart;
                     current != srcEnd;
                     ++current, ++dest) {
//#if defined __APPLE__ && defined __MACH__
                    *dest = static_cast<Y>(::sqrt((current->real() * current->real()) +
                                                  (current->imag() * current->imag())));
//#else
//          *dest = static_cast<Y>(std::abs(current));
//#endif
                    //*dest = static_cast<Y>(current->real());
                }
            }
        }
    };
}

#endif // SIGBLOCKS_GENERICCOPY_H
