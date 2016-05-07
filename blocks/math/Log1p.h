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

#ifndef SIGNALBLOCKS_LOG1P_H
#define SIGNALBLOCKS_LOG1P_H

#include "UnaryOperator.h"

#include <gsl/gsl_math.h>

namespace signalblocks {
    // computes  \log(1+x)
    template<class T>
    class Log1p
            : public UnaryOperator<T> {
    public:
        Log1p(std::string name)
                : UnaryOperator<T>(std::move(name)) {
        }

    protected: // UnaryOperator interface
        virtual T Compute(const T& arg1) {
            return gsl_log1p(arg1);
        }

    };
}

#endif // SIGNALBLOCKS_LOG1P_H
