// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGBLOCKS_ISTREAMSOURCE_H
#define SIGBLOCKS_ISTREAMSOURCE_H

#include "../../common/Port.h"
#include "../../common/TimeTick.h"
#include "../../common/logging.h"

#include <fstream>
#include <memory>


namespace sigblocks {
    template<class T>
    class IstreamSource
            : public Port<0, 1, T> {
    public:
        IstreamSource(std::string name, int blockSize)
                : Port<0, 1, T>(std::move(name), "An input stream source block"),
                  mLastTick(),
                  mpIstream(nullptr),
                  mLoopOver(true),
                  mBlockSize(blockSize) {
        }

        IstreamSource(std::string name,
                      int blockSize,
                      std::unique_ptr<std::istream> pIns)
                : Port<0, 1, T>(std::move(name), "An input stream source block"),
                  mLastTick(),
                  mpIstream(std::move(pIns)),
                  mLoopOver(true),
                  mBlockSize(blockSize) {
        }

        void SetStreamSource(std::unique_ptr<std::istream> pIns) {
            mpIstream.swap(pIns);
        }

        void Loop(bool loopOver) {
            mLoopOver = loopOver;
        }

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick) {
            if (mpIstream->eof() && !mLoopOver) {
                LOG_DEBUG("IstreamSource is done processing!\n");
                return; // dont do anything
            }
            if (mLastTick == timeTick) {
                return;  // already processed the event
            }
            mLastTick = timeTick;

            std::unique_ptr<T[]> data(new T[mBlockSize]);
            mpIstream->read(reinterpret_cast<char*>(data.get()), mBlockSize * sizeof(T));
            int bytes_read = static_cast<int>(mpIstream->gcount() / sizeof(char));
            if (bytes_read <= 0) {
                if (mpIstream->eof() && mLoopOver) {
                    mpIstream->clear();
                    mpIstream->seekg(0, std::ios::beg);
                    // lets try again
                    mpIstream->read(reinterpret_cast<char*>(data.get()), mBlockSize * sizeof(T));
                    bytes_read = static_cast<int>(mpIstream->gcount() / sizeof(char));
                    if (bytes_read <= 0) {
                        // still doesnt work, so give up
                        return;
                    }
                } else {
                    return;
                }
            }
            this->LeakData(0, std::move(data), bytes_read, timeTick);
            if (mpIstream->eof() && mLoopOver) {
                mpIstream->clear();
                mpIstream->seekg(0, std::ios::beg);
            }
        }

    private:
        TimeTick mLastTick;
        std::unique_ptr<std::istream> mpIstream;
        bool mLoopOver;
        int mBlockSize;
    };
}

#endif // SIGBLOCKS_ISTREAMSOURCE_H
