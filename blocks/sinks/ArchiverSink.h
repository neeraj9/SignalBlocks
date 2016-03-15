// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_ARCHIVERSINK_H
#define SIGBLOCKS_ARCHIVERSINK_H

#include "../../common/Port.h"

#include <list>
#include <vector>

namespace sigblocks {
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

#endif //SIGBLOCKS_ARCHIVERSINK_H
