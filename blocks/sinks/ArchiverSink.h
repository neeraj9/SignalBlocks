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

#ifndef SIGNALBLOCKS_ARCHIVERSINK_H
#define SIGNALBLOCKS_ARCHIVERSINK_H

#include "../../common/Port.h"

#include <list>
#include <vector>

namespace signalblocks {
    template<class T>
    class ArchiverSink
            : public Port<1, 0, T> {
    public:

        class ArchiveScalarInfo {
        public:

            ArchiveScalarInfo(const T& data, const TimeTick& timeTick)
                    : mData(data),
                      mTimeTick(timeTick) {
            }

            ArchiveScalarInfo(ArchiveScalarInfo&& other)
                    : mData(other.mData),
                      mTimeTick(other.mTimeTick) {
            }

            T mData;
            TimeTick mTimeTick;
        };

        class ArchiveVectorInfo {
        public:

            ArchiveVectorInfo(std::unique_ptr<T[]> data, int len, const TimeTick& timeTick)
                    : mData(std::move(data)),
                      mLen(len),
                      mTimeTick(timeTick) {
            }

            ArchiveVectorInfo(ArchiveVectorInfo&& other)
                    : mData(std::move(other.mData)),
                      mLen(other.mLen),
                      mTimeTick(other.mTimeTick) {
            }

            std::unique_ptr<T[]> mData;
            int mLen;
            TimeTick mTimeTick;
        };

        class ArchiveMatrixInfo {
        public:

            ArchiveMatrixInfo(std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& timeTick)
                    : mData(std::move(data)),
                      mDims(dims),
                      mTimeTick(timeTick) {
            }

            ArchiveMatrixInfo(ArchiveMatrixInfo&& other)
                    : mData(std::move(other.mData)),
                      mDims(std::move(other.mDims)),
                      mTimeTick(other.mTimeTick) {
            }

            std::unique_ptr<T[]> mData;
            std::vector<int> mDims;
            TimeTick mTimeTick;
        };


    public:
        ArchiverSink(std::string name)
                : Port<1, 0, T>(std::move(name), "An archiver sink.") {
        }

        const std::list<ArchiveScalarInfo>& GetScalarInfo() const {
            return mScalarStorage;
        }

        const std::list<ArchiveVectorInfo>& GetVectorInfo() const {
            return mVectorStorage;
        }

        const std::list<ArchiveMatrixInfo>& GetMatrixInfo() const {
            return mMatrixStorage;
        }

    public: // Port interface
        void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            mScalarStorage.push_back(ArchiveScalarInfo(data, startTime));
        }

        void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            mVectorStorage.push_back(ArchiveVectorInfo(std::move(data), len, startTime));
        }

        void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            mMatrixStorage.push_back(ArchiveMatrixInfo(std::move(data), dims, startTime));
        }

    private:
        std::list<ArchiveScalarInfo> mScalarStorage;
        std::list<ArchiveVectorInfo> mVectorStorage;
        std::list<ArchiveMatrixInfo> mMatrixStorage;
    };
}

#endif //SIGNALBLOCKS_ARCHIVERSINK_H
