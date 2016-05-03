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

#ifndef SIGBLOCKS_ARCSINH_H
#define SIGBLOCKS_ARCSINH_H

#include "UnaryOperator.h"

#include <gsl/gsl_math.h>

namespace signalblocks {
    // computes  \arcsinh(x)
    template<class T>
    class Arcsinh
            : public UnaryOperator<T> {
    public:
        Arcsinh(std::string name)
                : UnaryOperator<T>(std::move(name)) {
        }

    protected: // UnaryOperator interface
        virtual T Compute(const T& arg1) const {
            return gsl_asinh(arg1);
        }

    };
}

#endif // SIGBLOCKS_ARCSINH_H
