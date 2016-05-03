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

#ifndef SIGBLOCKS_MULTIPLEX_H
#define SIGBLOCKS_MULTIPLEX_H

#include "../../common/Port.h"

#include <assert.h>
#include <map>

namespace sigblocks {

    enum MultiplexPolicy {
        MULTIPLEX_SCALAR = 0,
        MULTIPLEX_VECTOR,
        MULTIPLEX_MATRIX
    };

    template <int N, class T, MultiplexPolicy P>
    class Multiplex
            : public Port<N, 1, T> {
    public:
        Multiplex(std::string name)
                : Port<N, 1, T>(std::move(name), "A Multiplex block.") {
            assert(0);
        }
    };

    // Multiplex scalar and nothing else
    template <int N, class T>
    class Multiplex<N, T, MULTIPLEX_SCALAR>
            : public Port<N, 1, T> {
    public:
        Multiplex(std::string name, T defaultValue)
                : Port<N, 1, T>(std::move(name), "A Multiplex block."),
                  mDefaultValue(defaultValue),
                  mStorage(),
                  mLastTick() {
        }

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.
            if (mLastTick == startTime) {
                mStorage[sourceIndex] = data;
            }

            if ((mStorage.size() == N) || (mLastTick != startTime)) {
                // time to push out the data
                std::unique_ptr<T[]> outdata(new T[N]);
                // always guaranteed to be ascending order iteration
                unsigned int index = 0;
                for (auto iter = mStorage.begin(); iter != mStorage.end(); ++iter) {
                    unsigned int key = iter->first;
                    for (unsigned int i = index; i < key; ++i) {
                        outdata[i] = mDefaultValue;
                    }
                    std::swap(outdata.get()[key], iter->second);
                    index = key + 1; // next key
                }
                mStorage.clear();
                for (unsigned int i = index; i < N; ++i) {
                    outdata[i] = mDefaultValue;
                }
                LeakData(0, std::move(outdata), N, mLastTick);
            }
            mLastTick = startTime;
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(0);
        }

        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            assert(0);
        }

    private:
        T mDefaultValue;
        std::map<unsigned int, T> mStorage;
        TimeTick mLastTick;
    };

    // Multiplex vector and nothing else
    template<int N, class T>
    class Multiplex<N, T, MULTIPLEX_VECTOR>
            : public Port<N, 1, T> {
    public:
        Multiplex(std::string name, T defaultValue)
                : Port<N, 1, T>(std::move(name), "A Multiplex block."),
                  mDefaultValue(defaultValue),
                  mStorage(),
                  mLastTick() {
        }

    private:
        class InternalStorage {
        public:
            InternalStorage(std::unique_ptr<T[]>&& data, int len)
                    : mData(std::move(data)),
                      mLen(len) {
            }

            InternalStorage(InternalStorage&& copy)
                    : mData(std::move(copy.mData)),
                      mLen(copy.mLen) {
            }

            InternalStorage& operator=(InternalStorage&& rhs) {
                    mData.swap(rhs.mData);
                    mLen = rhs.mLen;
            }

        public:
            std::unique_ptr<T[]> mData;
            int mLen;
        };

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(0);
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.
            if (mLastTick == startTime) {
                if (! mStorage.empty()) {
                    assert(mStorage.begin()->second.mLen == len);  // all vectors must of same length
                }
                InternalStorage entry(std::move(data), len);
                // const std::pair<std::map<unsigned int, sigblocks::Multiplex<N, T, MULTIPLEX_VECTOR>::InternalStorage>::iterator, bool>&
                auto p = mStorage.insert(std::make_pair(sourceIndex, std::move(entry)));
                assert(p.second);
            }

            if ((mStorage.size() == N) || (mLastTick != startTime)) {
                // time to push out the data
                int len_to_send = len;
                if (! mStorage.empty()) {
                    len_to_send = mStorage.begin()->second.mLen;
                }
                std::unique_ptr<T[]> outdata(new T[len_to_send * N]);
                // always guaranteed to be ascending order iteration
                unsigned int index = 0;
                for (auto iter = mStorage.begin(); iter != mStorage.end(); ++iter) {
                    unsigned int key = iter->first;
                    for (unsigned int i = index; i < key; ++i) {
                        for (int j = 0; j < len_to_send; ++j) {
                            outdata.get()[i * len_to_send + j] = mDefaultValue;
                        }
                    }
                    for (int j = 0; j < len_to_send; ++j) {
                        std::swap(outdata.get()[key * len_to_send + j], iter->second.mData.get()[j]);
                    }
                    index = key + 1; // next key
                }
                mStorage.clear();
                for (unsigned int i = index; i < N; ++i) {
                    for (int j = 0; j < len_to_send; ++j) {
                        outdata.get()[i * len_to_send + j] = mDefaultValue;
                    }
                }
                // output remains a vector
                this->LeakData(0, std::move(outdata), len_to_send * N, mLastTick);
            }

            // buffer data when not done already
            if (mLastTick != startTime) {
                mLastTick = startTime;
                InternalStorage entry(std::move(data), len);
                mStorage.insert(std::make_pair(sourceIndex, std::move(entry)));
            }

        }

        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            assert(0);
        }

    private:
        T mDefaultValue;
        std::map<unsigned int, Multiplex<N, T, MULTIPLEX_VECTOR>::InternalStorage> mStorage;
        TimeTick mLastTick;
    };

    // Multiplex matrix and nothing else
    template<int N, class T>
    class Multiplex<N, T, MULTIPLEX_MATRIX>
            : public Port<N, 1, T> {
    public:
        Multiplex(std::string name, T defaultValue)
                : Port<N, 1, T>(std::move(name), "A Multiplex block."),
                  mDefaultValue(defaultValue),
                  mStorage(),
                  mLastTick() {
        }

    private:
        class InternalStorage {
        public:
            InternalStorage(std::unique_ptr<T[]> data, const std::vector<int>& dims)
                    : mData(std::move(data)),
                      mDims(dims) {
            }

            InternalStorage(InternalStorage&& copy)
                    : mData(std::move(copy.mData)),
                      mDims(copy.mDims) {
            }

        public:
            std::unique_ptr<T[]> mData;
            std::vector<int> mDims;
        };

    protected: // Port interface
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(0);
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(0);
        }

        // TODO FIXME based on the matrix storage as specified in IPort.h
        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            assert(sourceIndex >= 0 || sourceIndex < N); // XXX change to an assertion library.
            assert(! dims.empty());
            int len = dims[0];
            for (size_t i = 1; i < dims.size(); ++i) {
                len *= dims[i];
            }
            assert(len > 0);

            if (mLastTick == startTime) {
                if (! mStorage.empty()) {
                    assert(mStorage.begin()->second.mDims == dims);  // all matrix must of same dimensions
                }
                InternalStorage entry(std::move(data), dims);
                // const std::pair<std::map<unsigned int, sigblocks::Multiplex<N, T, MULTIPLEX_VECTOR>::InternalStorage>::iterator, bool>&
                auto p = mStorage.insert(std::make_pair(sourceIndex, std::move(entry)));
                assert(p.second);
            }

            if ((mStorage.size() == N) || (mLastTick != startTime)) {
                // time to push out the data
                std::vector<int> outdims;
                int len_to_send = len;
                if (! mStorage.empty()) {
                    len_to_send = mStorage.begin()->second.mDims[0];
                    for (size_t i = 1; i < mStorage.begin()->second.mDims.size(); ++i) {
                        len_to_send *= mStorage.begin()->second.mDims[i];
                    }
                    // in order to avoid copy swap out the first index dims
                    outdims.swap(mStorage.begin()->second.mDims);
                } else {
                    outdims = dims;
                }
                // the last dimension will increase since data is multiplexed there
                outdims[outdims.size() - 1] *= N;

                std::unique_ptr<T[]> outdata(new T[len_to_send * N]);
                // always guaranteed to be ascending order iteration
                unsigned int index = 0;
                for (auto iter = mStorage.begin(); iter != mStorage.end(); ++iter) {
                    unsigned int key = iter->first;
                    for (unsigned int i = index; i < key; ++i) {
                        for (int j = 0; j < len_to_send; ++j) {
                            outdata.get()[i * len_to_send + j] = mDefaultValue;
                        }
                    }
                    for (int j = 0; j < len_to_send; ++j) {
                        std::swap(outdata.get()[key * len_to_send + j], iter->second.mData.get()[j]);
                    }
                    index = key + 1; // next key
                }
                mStorage.clear();
                for (unsigned int i = index; i < N; ++i) {
                    for (int j = 0; j < len_to_send; ++j) {
                        outdata.get()[i * len_to_send + j] = mDefaultValue;
                    }
                }
                // output remains a matrix with same dimension but the last dimension value increases

                LeakData(0, std::move(outdata), len_to_send * N, mLastTick);
            }

            // buffer data when not done already
            if (mLastTick != startTime) {
                InternalStorage entry(std::move(data), len);
                auto p = mStorage.insert(std::make_pair(sourceIndex, std::move(entry)));
                // since storage will be empty at this point so asserting is unnecessary
                // assert(p.second);
            }
            mLastTick = startTime;
        }

    private:
        T mDefaultValue;
        std::map<unsigned int, InternalStorage> mStorage;
        TimeTick mLastTick;
    };
}

#endif // SIGBLOCKS_MULTIPLEX_H
