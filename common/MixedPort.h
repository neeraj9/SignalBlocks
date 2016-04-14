// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_MIXEDPORT_H
#define SIGBLOCKS_MIXEDPORT_H

#include "IPort.h"
#include "TimeTick.h"
#include "Port.h"

namespace sigblocks {

    /// Note that we are using Port<1, M, T>, but actually there is no
    /// source connected to TransparentProxy
    template<int M, class T>
    class TransparentProxy
            : public Port<1, M, T> {
    protected: // override Port interface to act as a transparent proxy
        virtual void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            for (int i = 0; i < M; ++i) {
                this->LeakData(i, data, startTime);
            }
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            for (int i = 0; i < M; ++i) {
                this->LeakData(i, std::move(data), len, startTime);
            }
        }

        virtual void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            for (int i = 0; i < M; ++i) {
                this->LeakMatrix(i, std::move(data), dims, startTime);
            }
        }
    };

    // XXX specialze a case when N=1, M=1 and optimize it
    template<int N, int M, class TN, class TM>
    class MixedPort
            : public IPort<TN> {
    public:
        MixedPort() : mpSource(),
                      mInternalSource(new TransparentProxy<M, TM>()) {
            for (int i = 0; i < N; ++i) {
                mpSource[i] = nullptr;
            }
        }

        virtual ~MixedPort() {
        }

        std::shared_ptr<IPort<TM> >& GetAsSinkType() {
            return mInternalSource;
        }

    public: // IPort interface
        void SetSource(IPort<TN>* peer, int index) {
            assert(index < N); // XXX replace assert with better error handling
            mpSource[index] = peer;
        }

        virtual bool IsSourceConnected(int index) const {
            return mpSource[index] != nullptr;
        }

        virtual bool IsSinkConnected(int index) const {
            return false;  // direct sink is not valid, look for GetAsSinkType()->IsSinkConnected()
        }

        void DisconnectSource(IPort<TN>* peer) {
            for (int i = 0; i < N; ++i) {
                if (mpSource[i] == peer) {
                    mpSource[i] = 0;
                }
            }
        }

        /// use this interface when T is a primitive data type, ex. int, float, etc
        void ConsumeScalar(
                const IPort<TN>* pSender, const TN& data, const TimeTick& startTime) {
            for (int i = 0; i < N; ++i) {
                if (mpSource[i] == pSender) {
                    Process(i, data, startTime);
                    break;
                }
            }
        }

        void ConsumeVector(
                const IPort<TN>* pSender, std::unique_ptr<TN[]> data, int len, const TimeTick& startTime) {
            for (int i = 0; i < N; ++i) {
                if (mpSource[i] == pSender) {
                    Process(i, std::move(data), len, startTime);
                    break;
                }
            }
        }

        void ConsumeMatrix(const IPort<TN>* pSender,
                           std::unique_ptr<TN[]> data,
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
            // default implementation is to pass clock cycle
            mInternalSource->ClockCycle(timeTick);
        }

    private: // IPort interface (which should not be used because sink are of type TM)
        // NOT TO BE USED
        void SetSink(std::shared_ptr<IPort<TN> >& peer, int index) {
        }

    protected: // override these methods in derived class to do something useful
        /// use this interface when T is a primitive data type, ex. int, float, etc
        virtual void Process(int sourceIndex, const TN& data, const TimeTick& startTime) {
            // silently drop here, while derived class should do something
            // useful
        }

        virtual void Process(
                int sourceIndex, std::unique_ptr<TN[]> data, int len, const TimeTick& startTime) {
            // silently drop here, while derived class should do something
            // useful
        }

        virtual void ProcessMatrix(int sourceIndex,
                                   std::unique_ptr<TN[]> data,
                                   const std::vector<int>& dims,
                                   const TimeTick& startTime) {
            // silently drop here, while derived class should do something
            // useful
        }

    private:
        IPort<TN>* mpSource[N];
        std::shared_ptr<IPort<TM> > mInternalSource;
    };
}

#endif // SIGBLOCKS_MIXEDPORT_H
