// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "MixedPort.h"
#include "Port.h"

#include <iostream>
#include <assert.h>

using namespace sigblocks;
using namespace std;


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
    };
}

template<int N, int M, class TN, class TM>
MixedPort<N, M, TN, TM>::MixedPort()
        : mpSource(),
          mInternalSource(new TransparentProxy<M, TM>()) {
}

template<int N, int M, class TN, class TM>
MixedPort<N, M, TN, TM>::~MixedPort() {
}

template<int N, int M, class TN, class TM>
std::shared_ptr<IPort<TM> >& MixedPort<N, M, TN, TM>::GetAsSinkType() {
    return mInternalSource;
}

template<int N, int M, class TN, class TM>
void
MixedPort<N, M, TN, TM>::SetSource(IPort<TN>* peer, int index) {
    assert(index < N); // XXX replace assert with better error handling
    mpSource[index] = peer;
}

template<int N, int M, class TN, class TM>
void
MixedPort<N, M, TN, TM>::DisconnectSource(IPort<TN>* peer) {
    for (int i = 0; i < N; ++i) {
        if (mpSource[i] == peer) {
            mpSource[i] = 0;
        }
    }
}

template<int N, int M, class TN, class TM>
void
MixedPort<N, M, TN, TM>::ConsumeData(const IPort<TN>* pSender,
                                     const TN& data,
                                     const TimeTick& startTime) {
    for (int i = 0; i < N; ++i) {
        if (mpSource[i] == pSender) {
            Process(i, data, startTime);
            break;
        }
    }
}

template<int N, int M, class TN, class TM>
void
MixedPort<N, M, TN, TM>::ConsumeData(const IPort<TN>* pSender,
                                     std::unique_ptr<TN[]> data,
                                     int len,
                                     const TimeTick& startTime) {
    for (int i = 0; i < N; ++i) {
        if (mpSource[i] == pSender) {
            Process(i, std::move(data), len, startTime);
            break;
        }
    }
}


template<int N, int M, class TN, class TM>
void
MixedPort<N, M, TN, TM>::ClockCycle(const TimeTick& timeTick) {
    // default implementation is to pass clock cycle
    mInternalSource->ClockCycle(timeTick);
}

template<int N, int M, class TN, class TM>
void
MixedPort<N, M, TN, TM>::Process(int sourceIndex,
                                 const TN& data,
                                 const TimeTick& startTime) {
    // silently drop here, while derived class should do something
    // useful
}

template<int N, int M, class TN, class TM>
void
MixedPort<N, M, TN, TM>::Process(int sourceIndex,
                                 std::unique_ptr<TN[]> data,
                                 int len,
                                 const TimeTick& startTime) {
    // silently drop here, while derived class should do something
    // useful
}

//
// interface which are intentionally private to disable use
//

template<int N, int M, class TN, class TM>
void
MixedPort<N, M, TN, TM>::SetSink(std::shared_ptr<IPort<TN> >& peer, int index) {
}

////////////////////////////////////////////////////////////

#define INSTANTIATE_TEMPLATE(fromtype, totype) \
  template class sigblocks::MixedPort<1, 1, fromtype, totype>;

// Instantiate so that they can be used elsewhere
//INSTANTIATE_TEMPLATE(char, short)
//INSTANTIATE_TEMPLATE(short, int)
//INSTANTIATE_TEMPLATE(int, long)
//INSTANTIATE_TEMPLATE(unsigned char, unsigned short)
//INSTANTIATE_TEMPLATE(unsigned short, unsigned int)
//INSTANTIATE_TEMPLATE(unsigned int, unsigned long)
INSTANTIATE_TEMPLATE(int, float)

INSTANTIATE_TEMPLATE(int, double)
//INSTANTIATE_TEMPLATE(float, double)
