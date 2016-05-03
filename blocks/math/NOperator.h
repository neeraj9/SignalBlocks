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

#ifndef SIGBLOCKS_NOPERATOR_H
#define SIGBLOCKS_NOPERATOR_H

#include "../../common/Port.h"
#include "../../common/logging.h"

#include <memory>
#include <map>
#include <vector>


namespace sigblocks {
    template<int N, class T>
    class NOperator
            : public Port<N, 1, T> {
    public:
        NOperator(std::string name)
                : Port<N, 1, T>(std::move(name), "An operator block."),
                  mDefaultValue(),
                  mStorage(),
                  mDims(),
                  mLastTick() {
        }

        NOperator(std::string name, const T& defaultValue)
                : Port<N, 1, T>(std::move(name), "An operator block."),
                  mDefaultValue(defaultValue),
                  mStorage(),
                  mDims(),
                  mLastTick() {
        }
    protected:
        // compute and store the result in pArgs[0] or pArgs[x], where x is the first index
        // within pArgs which is non nullptr (to save unnecessary memory allocation).
        // Note that each one of the args is of dims dimension, that is pArgs[0] ... pArgs[N-1]
        // has dimensions dims.
        // For scalar dims.size() == 1 and dims[0] == 1.
        // For vector dims.size() == 1 and dims[0] > 1.
        // For matrix dims.size() > 1.
        //
        virtual void Compute(T* pArgs[N], const std::vector<int>& dims) = 0;

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < 2); // XXX change to an assertion library.
            if (mDims.empty()) {
                mDims.push_back(1);
            } else {
                assert(mDims.size() == 1);
                assert(mDims[0] == 1);
            }
            if (mLastTick == startTime) {
                //mStorage[sourceIndex] = data;
                std::unique_ptr<T[]> val(new T[1]);
                val.get()[0] = data; // optimize complex T with std::swap(val.get()[0], data);
                auto p = mStorage.insert(std::make_pair(sourceIndex, std::move(val)));
                assert(p.second);
                LOG_DEBUG("Added value to storage\n");
            }

            if ((mStorage.size() == N) || ((! mStorage.empty()) && mLastTick != startTime)) {
                // time to push out the data
                T* arguments[N] = {nullptr};
                for (auto iter = mStorage.begin(); iter != mStorage.end(); ++iter) {
                    arguments[iter->first] = iter->second.get();
                }
                for (int i = 0; i < N; ++i) {
                    if (arguments[i] == nullptr) {
                        LOG_DEBUG("Adding default value to storage at index=%d\n", i);
                        std::unique_ptr<T[]> val(new T[1]);
                        val.get()[0] = mDefaultValue;
                        arguments[i] = val.get();
                        mStorage.insert(std::make_pair(i, std::move(val)));
                    }
                }
                LOG_DEBUG("Computing and sending to downstream\n");
                this->Compute(arguments, mDims);
                this->LeakData(0, *arguments[0], mLastTick);
                mStorage.clear();
                mDims.clear();
            }
            if (mLastTick != startTime) {
                LOG_DEBUG("New Tick so buffer data\n");
                mLastTick = startTime;
                std::unique_ptr<T[]> val(new T[1]);
                val.get()[0] = data; // use in optimization std::swap(val.get()[0], data);
                mStorage.insert(std::make_pair(sourceIndex, std::move(val)));
            }
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < 2); // XXX change to an assertion library.
            if (mDims.empty()) {
                mDims.push_back(len);
            } else {
                // If you want this same block to be used alternatively with
                // scalar, vector or matrix during its lifetime then
                // run this validation only when mLastTick == startTime, else
                // accept the values received
                assert(mDims.size() == 1);
                assert(mDims[0] == len);
            }
            if (mLastTick == startTime) {
                auto p = mStorage.insert(std::make_pair(sourceIndex, std::move(data)));
                assert(p.second);
            }

            if ((mStorage.size() == N) || ((! mStorage.empty()) && mLastTick != startTime)) {
                // time to push out the data
                T* arguments[N] = {nullptr};
                for (auto iter = mStorage.begin(); iter != mStorage.end(); ++iter) {
                    arguments[iter->first] = iter->second.get();
                }
                for (int i = 0; i < N; ++i) {
                    if (arguments[i] == nullptr) {
                        std::unique_ptr<T[]> val(new T[mDims[0]]);
                        for (int j = 0; j < mDims[0]; ++j) {
                            val.get()[j] = mDefaultValue;
                        }
                        arguments[i] = val.get();
                        mStorage.insert(std::make_pair(i, std::move(val)));
                    }
                }
                this->Compute(arguments, mDims);
                // result is always stored in first argument, so move that down
                this->LeakData(0, std::move(mStorage[0]), mDims[0], mLastTick);
                mStorage.clear();
                mDims.clear();
            }
            if (mLastTick != startTime) {
                mLastTick = startTime;
                mStorage.insert(std::make_pair(sourceIndex, std::move(data)));
            }
        }

        virtual void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < 2); // XXX change to an assertion library.
            if (mDims.empty()) {
                mDims = dims;
            } else {
                // If you want this same block to be used alternatively with
                // scalar, vector or matrix during its lifetime then
                // run this validation only when mLastTick == startTime, else
                // accept the values received
                assert(mDims == dims);
            }
            if (mLastTick == startTime) {
                auto p = mStorage.insert(std::make_pair(sourceIndex, std::move(data)));
                assert(p.second);
            }

            if ((mStorage.size() == N) || ((! mStorage.empty()) && mLastTick != startTime)) {
                int len = mDims[0];
                for (size_t i = 1; i < mDims.size(); ++i) {
                    len *= mDims[i];
                }
                // time to push out the data
                T* arguments[N] = {nullptr};
                for (auto iter = mStorage.begin(); iter != mStorage.end(); ++iter) {
                    arguments[iter->first] = iter->second.get();
                }
                for (int i = 0; i < N; ++i) {
                    if (arguments[i] == nullptr) {
                        std::unique_ptr<T[]> val(new T[len]);
                        for (int j = 0; j < len; ++j) {
                            val.get()[j] = mDefaultValue;
                        }
                        arguments[i] = val.get();
                        mStorage.insert(std::make_pair(i, std::move(val)));
                    }
                }
                this->Compute(arguments, mDims);
                // result is always stored in first argument, so move that down
                this->LeakMatrix(0, std::move(mStorage[0]), mDims, mLastTick);
                mStorage.clear();
                mDims.clear();
            }
            if (mLastTick != startTime) {
                mLastTick = startTime;
                mStorage.insert(std::make_pair(sourceIndex, std::move(data)));
            }
        }

    private:
        T mDefaultValue;
        std::map<int, std::unique_ptr<T[]> > mStorage;
        std::vector<int> mDims;
        TimeTick mLastTick;
    };
}


#endif // SIGBLOCKS_NOPERATOR_H
