// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#include "WavSoundSource.h"

#include "../../common/logging.h"

using namespace sigblocks;

namespace {
    void ReadLittleEndianValue(std::istream* pIns, uint16_t& value) {
        char tmp[2];
        pIns->read(tmp, sizeof(tmp));
        value = static_cast<uint16_t>(
                ((tmp[1] & 0xff) << 8) |
                 (tmp[0] & 0xff));
    }
    void ReadLittleEndianValue(std::istream* pIns, uint32_t& value) {
        char tmp[4];
        pIns->read(tmp, sizeof(tmp));
        value = static_cast<uint32_t>(
                ((tmp[3] & 0xff) << 24) |
                ((tmp[2] & 0xff) << 16) |
                ((tmp[1] & 0xff) << 8) |
                (tmp[0] & 0xff));
    }
    void ReadLittleEndianValue(std::istream* pIns, uint64_t& value) {
        char tmp[8];
        pIns->read(tmp, sizeof(tmp));
        value = static_cast<uint64_t>(
                ((tmp[7] & 0xff) << 56) |
                ((tmp[6] & 0xff) << 48) |
                ((tmp[5] & 0xff) << 40) |
                ((tmp[4] & 0xff) << 32) |
                ((tmp[3] & 0xff) << 24) |
                ((tmp[2] & 0xff) << 16) |
                ((tmp[1] & 0xff) << 8) |
                (tmp[0] & 0xff));
    }
}


template <class T>
WavSoundSource<T>::WavSoundSource(int blockSize, std::unique_ptr<std::istream> pIns)
        : mLastTick(),
          mpIstream(std::move(pIns)),
          mLoopOver(true),
          mBlockSize(blockSize),
          mHeader(),
          mIsWavDataLittleEndian(true) {
    // read the audio header
    mpIstream->read(reinterpret_cast<char*>(&mHeader.mChunkId[0]), sizeof(mHeader.mChunkId));
    ReadLittleEndianValue(mpIstream.get(), mHeader.mChunkSize);
    mpIstream->read(reinterpret_cast<char*>(&mHeader.mFormat[0]), sizeof(mHeader.mFormat));
    mpIstream->read(reinterpret_cast<char*>(&mHeader.mSubchunk1Id[0]), sizeof(mHeader.mSubchunk1Id));
    ReadLittleEndianValue(mpIstream.get(), mHeader.mSubchunk1Size);
    ReadLittleEndianValue(mpIstream.get(), mHeader.mAudioFormat);
    ReadLittleEndianValue(mpIstream.get(), mHeader.mNumChannels);
    ReadLittleEndianValue(mpIstream.get(), mHeader.mSampleRate);
    ReadLittleEndianValue(mpIstream.get(), mHeader.mByteRate);
    ReadLittleEndianValue(mpIstream.get(), mHeader.mBlockAlign);
    ReadLittleEndianValue(mpIstream.get(), mHeader.mBitsPerSample);
    mpIstream->read(reinterpret_cast<char*>(&mHeader.mSubchunk2Id[0]), sizeof(mHeader.mSubchunk2Id));
    ReadLittleEndianValue(mpIstream.get(), mHeader.mSubchunk2Size);
    if (mpIstream->eof()) {
        LOG_ERROR("Error reading the complete header");
        assert(0);
    }

    if (mHeader.mChunkId[0] == 'R' &&
            mHeader.mChunkId[1] == 'I' &&
            mHeader.mChunkId[2] == 'F' &&
            mHeader.mChunkId[3] == 'F') {
        mIsWavDataLittleEndian = true;
    } else if (mHeader.mChunkId[0] == 'R' &&
            mHeader.mChunkId[1] == 'I' &&
            mHeader.mChunkId[2] == 'F' &&
            mHeader.mChunkId[3] == 'X') {
        mIsWavDataLittleEndian = false;
    } else {
        LOG_ERROR("Invalid chunk id in source stream");
        assert(0);
    }

    // expecting PCM wav file
    assert(mHeader.mAudioFormat == 1);

    // the storage capacity must be at least as much
    // required by the source wav file data samples.
    assert(sizeof(T) >= (mHeader.mBitsPerSample / 8));
}

template <class T>
void
WavSoundSource<T>::Loop(bool loopOver) {
    mLoopOver = loopOver;
}

template <class T>
const typename WavSoundSource<T>::PcmWavHeader&
WavSoundSource<T>::GetPcmWavHeader() const {
    return mHeader;
}

template <class T>
void
WavSoundSource<T>::ClockCycle(const TimeTick& timeTick) {
    if (mpIstream->eof() && !mLoopOver) {
        LOG_ERROR("WavSoundSource is done processing!\n");
        return; // dont do anything
    }
    if (mLastTick == timeTick) {
        return;  // already processed the event
    }
    mLastTick = timeTick;

    // always generate matrix data, where first dimension is the data and
    // second dimension is the channel
    std::unique_ptr<T[]> data(new T[mBlockSize * mHeader.mNumChannels]);

    int byte_per_sample = (mHeader.mBitsPerSample / 8);
    // could potentially allocate space on stack for tmp, but then
    // lets not worry too much about the overall size right now.
    std::unique_ptr<char[]> tmp(new char[mBlockSize * mHeader.mNumChannels * byte_per_sample]);
    mpIstream->read(tmp.get(), mBlockSize * mHeader.mNumChannels * byte_per_sample);
    int bytes_read = mpIstream->gcount() / sizeof(char);
    // copy to target data type
    int tmp_offset = 0;
    for (int i = 0; i < mBlockSize * mHeader.mNumChannels; ++i) {
        // TODO: data[i] = tmp[i]; (look at byte_per_sample)
        switch (byte_per_sample) {
            case 1: // 8 bits
                data[i] = tmp[tmp_offset++];
                break;
            case 2: // 16
            {
                if (mIsWavDataLittleEndian) {
                    data[i] = ((tmp[tmp_offset + 1] << 8) | (tmp[tmp_offset] & 0x00ff));
                } else {
                    data[i] = ((tmp[tmp_offset] << 8) | (tmp[tmp_offset + 1] & 0x00ff));
                }
                tmp_offset += 2;
                break;
            }
            case 4:  // 32 bits
            {
                if (mIsWavDataLittleEndian) {
                    data[i] = ((tmp[tmp_offset + 3] << 24) |
                               (tmp[tmp_offset + 2] << 16) |
                               (tmp[tmp_offset + 1] << 8) |
                               (tmp[tmp_offset] & 0x00ff));
                } else {
                    data[i] = ((tmp[tmp_offset] << 24) |
                               (tmp[tmp_offset + 1] << 16) |
                               (tmp[tmp_offset + 2] << 8) |
                               (tmp[tmp_offset + 3] & 0x00ff));
                }
                tmp_offset += 4;
                break;
            }
            case 8:  // 64 bits
            {
                if (mIsWavDataLittleEndian) {
                    data[i] = ((tmp[tmp_offset + 7] << 56) |
                               (tmp[tmp_offset + 6] << 48) |
                               (tmp[tmp_offset + 5] << 40) |
                               (tmp[tmp_offset + 4] << 32) |
                               (tmp[tmp_offset + 3] << 24) |
                               (tmp[tmp_offset + 2] << 16) |
                               (tmp[tmp_offset + 1] << 8) |
                               (tmp[tmp_offset] & 0x00ff));
                } else {
                    data[i] = ((tmp[tmp_offset] << 56) |
                               (tmp[tmp_offset + 1] << 48) |
                               (tmp[tmp_offset + 2] << 40) |
                               (tmp[tmp_offset + 3] << 32) |
                               (tmp[tmp_offset + 4] << 24) |
                               (tmp[tmp_offset + 5] << 16) |
                               (tmp[tmp_offset + 6] << 8) |
                               (tmp[tmp_offset + 7] & 0x00ff));
                }
                tmp_offset += 8;
                break;
            }
            default:
            {
                LOG_ERROR("Unsupported wav mHeader.mBitsPerSample=%d\n", mHeader.mBitsPerSample);
                assert(0);
            }
        }
    }
    std::vector<int> dims = {mHeader.mNumChannels,
                             bytes_read / mHeader.mNumChannels};
    this->LeakMatrix(0, std::move(data), dims, timeTick);
    if (mpIstream->eof() && mLoopOver) {
        mpIstream->clear();
        // skip over the header bytes
        mpIstream->seekg(PCM_WAV_HEADER_BYTES, std::ios::beg);
    }
}
