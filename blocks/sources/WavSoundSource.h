// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#ifndef SIGNALBLOCKS_WAVSOUNDSOURCE_H
#define SIGNALBLOCKS_WAVSOUNDSOURCE_H

#include "../../common/Port.h"

#include <fstream>
#include <string>
#include <stdint.h>

namespace sigblocks {
    template <class T>
    class WavSoundSource
            : public Port<0, 1, T> {
    public:
        WavSoundSource(int blockSize, std::unique_ptr<std::istream> pIns);

        void Loop(bool loopOver);

        // WAV PCM header
        static const int PCM_WAV_HEADER_BYTES = 44;

        struct PcmWavHeader {
            uint8_t mChunkId[4];
            uint32_t mChunkSize;
            uint8_t mFormat[4];
            uint8_t mSubchunk1Id[4];
            uint32_t mSubchunk1Size;
            uint16_t mAudioFormat;
            uint16_t mNumChannels;
            uint32_t mSampleRate;
            uint32_t mByteRate;
            uint16_t mBlockAlign;
            uint16_t mBitsPerSample;
            uint8_t mSubchunk2Id[4];
            uint32_t mSubchunk2Size;
        };

        const PcmWavHeader& GetPcmWavHeader() const;

    public:  // override Port interfaces

        virtual void ClockCycle(const TimeTick& timeTick);

    private:
        TimeTick mLastTick;
        std::unique_ptr<std::istream> mpIstream;
        bool mLoopOver;
        int mBlockSize;
        PcmWavHeader mHeader;
        bool mIsWavDataLittleEndian;
    };

}

#endif //SIGNALBLOCKS_WAVSOUNDSOURCE_H
