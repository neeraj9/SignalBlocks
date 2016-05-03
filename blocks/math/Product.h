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

#ifndef SIGBLOCKS_PRODUCT_H
#define SIGBLOCKS_PRODUCT_H

#include "NOperator.h"

namespace signalblocks {
    template<int N, class T>
    class Product
            : public NOperator<N, T> {
    public:
        Product(std::string name)
                : NOperator<N, T>(std::move(name)) {
        }

    protected: // NOperator interface

        virtual void Compute(T* pArgs[N], const std::vector<int>& dims) {
            T result = *pArgs[0];
            for (int i = 1; i < N; ++i) {
                result *= (*pArgs[i]);  // TODO handle saturate
            }
            *pArgs[0] = result;
        }
    };
}

#endif // SIGBLOCKS_PRODUCT_H
