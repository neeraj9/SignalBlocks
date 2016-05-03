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

#ifndef SIGBLOCKS_UNARYOPERATOR_H
#define SIGBLOCKS_UNARYOPERATOR_H

#include "../../common/Port.h"

namespace sigblocks {
    template<class T>
    class UnaryOperator
            : public Port<1, 1, T> {
    public:
        UnaryOperator(std::string name)
                : Port<1, 1, T>(std::move(name), "A unary operator") {
        }

    protected:
        virtual T Compute(const T& arg1) const = 0;

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX change to an assertion library.
            T output = this->Compute(data);
            this->LeakData(0, output, startTime);
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX change to an assertion library.
            for (int i = 0; i < len; ++i) {
                T output = this->Compute(data.get()[i]);
                // use swap instead of assignment operator to avoid copy for complex data type T,
                // but the user defined data type T should define swap() method to get the benefit.
                std::swap(data.get()[i], output);
            }
            this->LeakData(0, std::move(data), len, startTime);

        }

        virtual void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(sourceIndex == 0); // XXX change to an assertion library.
            assert(! dims.empty());
            int len = dims[0];
            for (size_t i = 1; i < dims.size(); ++i) {
                len *= dims[i];
            }
            assert(len > 0);
            for (int i = 0; i < len; ++i) {
                T output = this->Compute(data.get()[i]);
                // use swap instead of assignment operator to avoid copy for complex data type T,
                // but the user defined data type T should define swap() method to get the benefit.
                std::swap(data.get()[i], output);
            }
            this->LeakMatrix(0, std::move(data), dims, startTime);
        }
    };
}

#endif //SIGBLOCKS_UNARYOPERATOR_H
