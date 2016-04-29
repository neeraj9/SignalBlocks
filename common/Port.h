// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_PORT_H
#define SIGBLOCKS_PORT_H

#include "IPort.h"
#include "TimeTick.h"
#include "BlockManager.h"

#include <assert.h>
#include <functional>
#include <memory>
#include <string>

namespace sigblocks {
    // XXX specialze a case when N=1, M=1 and optimize it
    template<int N, int M, class T>
    class Port
            : public IPort<T> {
    public:
        Port(std::string name, std::string description)
                : mBlockInfo(std::move(name), std::move(description), TypeToBt<T>::ToType(), N, M),
                  mpSource(),
                  mpSink() {

            BlockManager::BlockInfoFuncType cb = std::bind(&Port<N, M, T>::GetBlockInfo,
                                                           this);
            mBlockInfo.mId = BlockManager::Getinstance()->Add(std::move(cb));

            for (int i = 0; i < N; ++i) {
                mpSource[i] = nullptr;
            }
        }

        virtual ~Port() {
            for (int i = 0; i < M; ++i) {
                if (mpSink[i]) {
                    mpSink[i]->DisconnectSource(this);
                }
            }
        }

    public: // IPort interface
        const BlockManager::BlockInfo& GetBlockInfo() {
            return mBlockInfo;
        }

        void SetSource(IPort<T>* peer, int index) {
            assert(index < N); // XXX replace assert with better error handling
            mpSource[index] = peer;
        }
        virtual bool IsSourceConnected(int index) const {
            return mpSource[index] != nullptr;
        }

        void SetSink(std::shared_ptr<IPort<T> >& peer, int index) {
            assert(index < M);
            mpSink[index] = peer;
        }

        virtual bool IsSinkConnected(int index) const {
            return mpSink[index].get() != nullptr;
        }

        void DisconnectSource(IPort<T>* peer) {
            for (int i = 0; i < N; ++i) {
                if (mpSource[i] == peer) {
                    mpSource[i] = 0;
                }
            }
        }

        /// use this interface when T is a primitive data type, ex. int, float, etc
        void ConsumeScalar(
                const IPort<T>* pSender, const T& data, const TimeTick& startTime) {
            for (int i = 0; i < N; ++i) {
                if (mpSource[i] == pSender) {
                    Process(i, data, startTime);
                    break;
                }
            }
        }

        void ConsumeVector(
                const IPort<T>* pSender, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            for (int i = 0; i < N; ++i) {
                if (mpSource[i] == pSender) {
                    Process(i, std::move(data), len, startTime);
                    break;
                }
            }
        }

        void ConsumeMatrix(const IPort<T>* pSender,
                           std::unique_ptr<T[]> data,
                           const std::vector<int>& dims,
                           const TimeTick& startTime) {
            for (int i = 0; i < N; ++i) {
                if (mpSource[i] == pSender) {
                    ProcessMatrix(i, std::move(data), dims, startTime);
                    break;
                }
            }
        }

        void ClockCycle(const TimeTick& timeTick) {
            // default implementation is to pass clock cycle to the sinks
            for (int i = 0; i < M; ++i) {
                mpSink[i]->ClockCycle(timeTick);
            }
        }

    protected:
        /// use this interface when T is a primitive data type, ex. int, float, etc
        void LeakData(
                int index, const T& data, const TimeTick& startTime) {
            assert(index < M);
            assert(0 <= index);
            if (mpSink[index]) {
                mpSink[index]->ConsumeScalar(this, data, startTime);
            }
        }

        void LeakData(
                int index, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(index < M);
            assert(0 <= index);
            if (mpSink[index]) {
                mpSink[index]->ConsumeVector(this, std::move(data), len, startTime);
            }
        }

        void LeakMatrix(int index,
                        std::unique_ptr<T[]> data,
                        const std::vector<int>& dims,
                        const TimeTick& startTime) {
            assert(index < M);
            assert(0 <= index);
            if (mpSink[index]) {
                mpSink[index]->ConsumeMatrix(this, std::move(data), dims, startTime);
            }
        }

        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            // silently drop
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            // silently drop
        }

        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            // silently drop
        }

    private:
        BlockManager::BlockInfo mBlockInfo;
        IPort<T>* mpSource[N];
        std::shared_ptr<IPort<T> > mpSink[M];
    };

    /// Partial Template Specialization for a information source
    template<int M, class T>
    class Port<0, M, T>
            : public IPort<T> {
    public:
        Port(std::string name, std::string description)
                : mBlockInfo(std::move(name), std::move(description), TypeToBt<T>::ToType(), 0, M),
                  mpSink() {
            BlockManager::BlockInfoFuncType cb = std::bind(&Port<0, M, T>::GetBlockInfo,
                                                           this);
            mBlockInfo.mId = BlockManager::Getinstance()->Add(std::move(cb));
        }

        virtual ~Port() {
            for (int i = 0; i < M; ++i) {
                if (mpSink[i]) {
                    mpSink[i]->DisconnectSource(this);
                }
            }
        }

    private: // IPort interface which is not allowed!
        // NOT ALLOWED to be used
        void SetSource(IPort<T>* peer, int index) {
        }

        void DisconnectSource(IPort<T>* peer) {
        }

        /// use this interface when T is a primitive data type, ex. int, float, etc
        void ConsumeScalar(
                const IPort<T>* pSender, const T& data, const TimeTick& startTime) {
        }

        void ConsumeVector(
                const IPort<T>* pSender, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
        }

        void ConsumeMatrix(const IPort<T>* pSender,
                           std::unique_ptr<T[]> data,
                           const std::vector<int>& dims,
                           const TimeTick& startTime) {
        }

    public: // IPort interface
        void SetSink(std::shared_ptr<IPort<T> >& peer, int index) {
            assert(index < M);
            mpSink[index] = peer;
        }

        virtual bool IsSourceConnected(int index) const {
            return false;
        }

        virtual bool IsSinkConnected(int index) const {
            return mpSink[index].get() != nullptr;
        }

        void ClockCycle(const TimeTick& timeTick) {
            // default implementation is to pass clock cycle
            for (int i = 0; i < M; ++i) {
                mpSink[i]->ClockCycle(timeTick);
            }
        }

        const BlockManager::BlockInfo& GetBlockInfo() {
            return mBlockInfo;
        }

    protected:
        /// use this interface when T is a primitive data type, ex. int, float, etc
        void LeakData(
                int index, const T& data, const TimeTick& startTime) {
            assert(index < M);
            assert(0 <= index);
            if (mpSink[index]) {
                mpSink[index]->ConsumeScalar(this, data, startTime);
            }
        }

        void LeakData(
                int index, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(index < M);
            assert(0 <= index);
            if (mpSink[index]) {
                mpSink[index]->ConsumeVector(this, std::move(data), len, startTime);
            }
        }

        void LeakMatrix(int index,
                        std::unique_ptr<T[]> data,
                        const std::vector<int>& dims,
                        const TimeTick& startTime) {
            assert(index < M);
            assert(0 <= index);
            if (mpSink[index]) {
                mpSink[index]->ConsumeMatrix(this, std::move(data), dims, startTime);
            }
        }

    private:
        BlockManager::BlockInfo mBlockInfo;
        std::shared_ptr<IPort<T> > mpSink[M];
    };

    /// Partial Template Specialization for a information sink
    template<int N, class T>
    class Port<N, 0, T>
            : public IPort<T> {
    public:
        Port(std::string name, std::string description)
                : mBlockInfo(std::move(name), std::move(description), TypeToBt<T>::ToType(), N, 0),
                  mpSource() {
            BlockManager::BlockInfoFuncType cb = std::bind(&Port<N, 0, T>::GetBlockInfo,
                                                           this);
            mBlockInfo.mId = BlockManager::Getinstance()->Add(std::move(cb));

            for (int i = 0; i < N; ++i) {
                mpSource[i] = nullptr;
            }
        }

        virtual ~Port() {
        }

    private: // IPort interface which is not allowed
        void SetSink(std::shared_ptr<IPort<T> >& peer, int index) {
        }

    public: // IPort interface
        void SetSource(IPort<T>* peer, int index) {
            assert(index < N); // XXX replace assert with better error handling
            mpSource[index] = peer;
        }

        virtual bool IsSourceConnected(int index) const {
            return mpSource[index] != nullptr;
        }

        virtual bool IsSinkConnected(int index) const {
            return false;
        }

        void DisconnectSource(IPort<T>* peer) {
            for (int i = 0; i < N; ++i) {
                if (mpSource[i] == peer) {
                    mpSource[i] = 0;
                }
            }
        }

        /// use this interface when T is a primitive data type, ex. int, float, etc
        void ConsumeScalar(
                const IPort<T>* pSender, const T& data, const TimeTick& startTime) {
            for (int i = 0; i < N; ++i) {
                if (mpSource[i] == pSender) {
                    Process(i, data, startTime);
                    break;
                }
            }
        }

        void ConsumeVector(
                const IPort<T>* pSender, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            for (int i = 0; i < N; ++i) {
                if (mpSource[i] == pSender) {
                    Process(i, std::move(data), len, startTime);
                    break;
                }
            }
        }

        void ConsumeMatrix(const IPort<T>* pSender,
                           std::unique_ptr<T[]> data,
                           const std::vector<int>& dims,
                           const TimeTick& startTime) {
            for (int i = 0; i < N; ++i) {
                if (mpSource[i] == pSender) {
                    ProcessMatrix(i, std::move(data), dims, startTime);
                    break;
                }
            }
        }

        void ClockCycle(const TimeTick& timeTick) {
            // no one to pass the clock cycle, so ignore
        }

        const BlockManager::BlockInfo& GetBlockInfo() {
            return mBlockInfo;
        }

    protected:
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            // silently drop
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            // silently drop
        }

        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<T[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            // silently drop
        }

    private:
        BlockManager::BlockInfo mBlockInfo;
        IPort<T>* mpSource[N];
    };

    /// Partial Template Specialization for a null port
    template<class T>
    class Port<0, 0, T>
            : public IPort<T> {
    public:
        Port(std::string name, std::string description)
                : mBlockInfo(std::move(name), std::move(description), TypeToBt<T>::ToType(), 0, 0) {
            BlockManager::BlockInfoFuncType cb = std::bind(&Port<0, 0, T>::GetBlockInfo,
                                                           this);
            mBlockInfo.mId = BlockManager::Getinstance()->Add(std::move(cb));
        }

        virtual ~Port() {
        }

    private: // IPort interface not allowed
        void SetSource(IPort<T>* peer, int index);

        void SetSink(std::shared_ptr<IPort<T> >& peer, int index);

        void DisconnectSource(IPort<T>* peer);

    public: // IPort interface (XXX do we need to use these?)
        virtual bool IsSourceConnected(int index) const {
            return false;
        }

        virtual bool IsSinkConnected(int index) const {
            return false;
        }

        /// use this interface when T is a primitive data type, ex. int, float, etc
        void ConsumeScalar(
                const IPort<T>* pSender, const T& data, const TimeTick& startTime) {
        }

        void ConsumeVector(
                const IPort<T>* pSender, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
        }

        void ConsumeMatrix(const IPort<T>* pSender,
                           std::unique_ptr<T[]> data,
                           const std::vector<int>& dims,
                           const TimeTick& startTime) {
        }

        void ClockCycle(const TimeTick& timeTick) {
            // no one to pass the clock cycle so ignore
        }

        const BlockManager::BlockInfo& GetBlockInfo() {
            return mBlockInfo;
        }

    private:
        BlockManager::BlockInfo mBlockInfo;
    };
}

#endif // SIGBLOCKS_PORT_H
