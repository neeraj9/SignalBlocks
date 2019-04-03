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

#ifndef SIGNALBLOCKS_CONSTPOW_H
#define SIGNALBLOCKS_CONSTPOW_H

#include "UnaryOperator.h"

#include <cmath>

namespace signalblocks {
    // computes  x^P, where P is a constant
    template<class T, int P>
    class ConstPow
            : public UnaryOperator<T> {
    public:
        ConstPow(std::string name)
                : UnaryOperator<T>(std::move(name)) {
        }

    protected: // UnaryOperator interface
        virtual T Compute(const T& arg1) {
            return std::pow(arg1, P);
        }

    };

    // Partial template specialization for optimized
    // power function.
    //
    // The c-equivalent functions are not optimized for
    // lower power, so template specialization is employed
    // to take advantage of specific gsl functions.
    //
    // Note to improve efficiency these
    // functions do not check for overflow or underflow conditions.
    //
    // computes  x^P, where P is a constant
#define OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(P) \
  template <class T> \
  class ConstPow<T, P> \
    : public UnaryOperator<T> \
  { \
  public: \
    ConstPow(std::string name) \
        : UnaryOperator<T>(std::move(name)) { \
    } \
  protected: \
    virtual T Compute(const T& arg1) \
    { \
      return gsl_pow_##P(arg1); \
    } \
  };

    OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(2);

    OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(3);

    OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(4);

    OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(5);

    OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(6);

    OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(7);

    OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(8);

    OPTIMIZED_POW_TEMPLATE_SPECIALIZATION(9);

}

#endif // SIGNALBLOCKS_CONSTPOW_H
