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

#include "PythonNOperator.h"

using namespace signalblocks;
using namespace std;

#define DESCRIPTION "An system implemented in python."

template<int N, class T>
PythonNOperator<N, T>::PythonNOperator(std::string name,
                                       std::shared_ptr<PythonPlugin>& plugin,
                                       const std::string& path,
                                       const std::string& moduleName,
                                       const std::string& functionName)
        : Port<N, 1, T>(std::move(name), DESCRIPTION),
          mPlugin(plugin),
          mPath(path),
          mModuleName(moduleName),
          mFunctionName(functionName),
          mPythonSource() {
}

template<int N, class T>
PythonNOperator<N, T>::PythonNOperator(
        std::string name,
        std::shared_ptr<PythonPlugin>& plugin,
        const std::string& path,
        const std::string& pysource)
        : Port<N, 1, T>(std::move(name), DESCRIPTION),
          mPlugin(plugin),
          mPath(path),
          mModuleName(),
          mFunctionName(),
          mPythonSource(pysource) {
}

template<int N, class T>
void
PythonNOperator<N, T>::Process(int sourceIndex, const T& data, const TimeTick& startTime) {
    assert(sourceIndex >= 0 || sourceIndex <= N); // XXX change to an assertion library.
    mDataPort[sourceIndex].push_back(data);
    mDataPortTime[sourceIndex].push_back(startTime);

    size_t min_elements = mDataPort[0].size();
    for (int i = 1; i < N; ++i) {
        const size_t new_min_elements = mDataPort[i].size();
        if (new_min_elements < min_elements) {
            min_elements = new_min_elements;
        }
    }

    if (min_elements <= 0) {
        return;
    }

    TimeTick min_time = mDataPortTime[0].front();
    for (int i = 1; i < N; ++i) {
        // We are sure that min_elements > 0 so no need to check for
        // the size of list before getting its front value.
        const TimeTick new_min_time = mDataPortTime[i].front();
        if (new_min_time < min_time) // operator< is better than operator>
            // see TimeTick for details
        {
            min_time = new_min_time;
        }
    }

    // assert(min_elements > 0);
    std::vector<T> arguments(N); // pre-allocate space
    for (int i = 0; i < N; ++i) {
        if (mDataPortTime[i].front() == min_time) {
            arguments[i] = mDataPort[i].front();
            mDataPort[i].pop_front();
            mDataPortTime[i].pop_front();
        }
        else {
            arguments[i] = 0; // insert zero for unmatched-rate (XXX)
            // unmatched rate is a big topic and should be carefully thought
            // and documented. XXX
        }
    }
    T result = Compute(arguments);
    this->LeakData(0, result, min_time);
}

template<int N, class T>
void
PythonNOperator<N, T>::Process(
        int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
    assert(0); // FIXME
}

#define INSTANTIATE_TYPES_FOR_N_INPUTS(NUM) \
  template class PythonNOperator<NUM, long>; \
  template class PythonNOperator<NUM, unsigned long>; \
  template class PythonNOperator<NUM, int>; \
  template class PythonNOperator<NUM, unsigned int>; \
  template class PythonNOperator<NUM, short>; \
  template class PythonNOperator<NUM, unsigned short>; \
  template class PythonNOperator<NUM, char>; \
  template class PythonNOperator<NUM, unsigned char>; \
  template class PythonNOperator<NUM, float>; \
  template class PythonNOperator<NUM, double>;

namespace signalblocks {
// XXX allow max of two inputs for now.
    INSTANTIATE_TYPES_FOR_N_INPUTS(1)

    INSTANTIATE_TYPES_FOR_N_INPUTS(2)
}
