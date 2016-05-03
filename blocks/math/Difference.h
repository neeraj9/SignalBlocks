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

#ifndef SIGBLOCKS_DIFFERENCE_H
#define SIGBLOCKS_DIFFERENCE_H

#include "NOperator.h"

namespace signalblocks {
    // implement a(n) - b(n) - c(n) - ...
    template<int N, class T>
    class Difference
            : public NOperator<N, T> {
    public:
        Difference(std::string name)
                : NOperator<N, T>(std::move(name)) {
        }

    protected: // NOperator interface
        // args[0], ... args[N] = a(n), b(n), ...
        // implement args[0] - args[1] - ... args[N-1]
        virtual void Compute(T* pArgs[N], const std::vector<int>& dims) {
            int len = dims[0];
            for (size_t i = 1; i < dims.size(); ++i) {
                len *= dims[i];
            }
            for (int i = 0; i < len; ++i) {
                T result = pArgs[0][i];
                for (int j = 1; j < N; ++j) {
                    result -= pArgs[j][i];  // TODO handle underflow
                }
                pArgs[0][i] = result;
            }
        }
    };
}

#endif // SIGBLOCKS_DIFFERENCE_H
