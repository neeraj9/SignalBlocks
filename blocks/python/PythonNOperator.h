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

#ifndef SIGNALBLOCKS_PYTHONNOPERATOR_H
#define SIGNALBLOCKS_PYTHONNOPERATOR_H

#include "../plugins/PythonPlugin.h"
#include "../common/Port.h"

#include <memory>
#include <list>
#include <vector>

namespace signalblocks {
    template<int N, class T>
    class PythonNOperator
            : public Port<N, 1, T> {
    public:
        PythonNOperator(std::string name,
                        std::shared_ptr<PythonPlugin>& plugin,
                        const std::string& path,
                        const std::string& moduleName,
                        const std::string& functionName);

        PythonNOperator(std::string name,
                        std::shared_ptr<PythonPlugin>& plugin,
                        const std::string& path,
                        const std::string& pysource);

    protected:
        virtual T Compute(const std::vector<T>& args) const = 0;

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime);

    private:
        std::shared_ptr<PythonPlugin> mPlugin;
        // The following can be const but its better that we make them
        // modifiable for later change by GUI/etc.
        std::string mPath;
        std::string mModuleName;
        std::string mFunctionName;
        std::string mPythonSource;

        std::list<T> mDataPort[N];
        std::list<TimeTick> mDataPortTime[N];
    };
}

#endif // SIGNALBLOCKS_PYTHONNOPERATOR_H
