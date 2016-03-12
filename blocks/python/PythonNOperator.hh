// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef sigblocks_math_PythonNOperator_hh
#define sigblocks_math_PythonNOperator_hh

#include "../plugins/PythonPlugin.hh"
#include "../common/Port.hh"

#include <memory>
#include <list>
#include <vector>

namespace sigblocks {
    template<int N, class T>
    class PythonNOperator
            : public Port<N, 1, T> {
    public:
        PythonNOperator(std::shared_ptr<PythonPlugin>& plugin,
                        const std::string& path,
                        const std::string& moduleName,
                        const std::string& functionName);

        PythonNOperator(std::shared_ptr<PythonPlugin>& plugin,
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

#endif // sigblocks_math_PythonNOperator_hh
