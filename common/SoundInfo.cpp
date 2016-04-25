// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#include "SoundInfo.h"
#include "logging.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <istream>
#include <unistd.h>

using namespace sigblocks;

namespace {
    static sf_count_t SfVioGetFileLen(void *pData) {
        // get file len
        std::istream* stream = static_cast<std::istream*>(pData);
        sf_count_t pos = stream->tellg();
        stream->seekg(0, stream->end);
        sf_count_t len = stream->tellg();
        stream->seekg(pos, stream->beg);
        return len;
    }

    static sf_count_t SfVioSeek(sf_count_t offset, int whence, void *pData) {
        // seek to position
        std::istream* stream = static_cast<std::istream*>(pData);
        switch (whence) {
            case SEEK_CUR:
                stream->seekg(offset, stream->cur);
                break;
            case SEEK_SET:
                stream->seekg(offset, stream->beg);
                break;
            case SEEK_END:
                stream->seekg(offset, stream->end);
                break;
            default:
                assert(0);  // not valid
        }
        sf_count_t new_offset = stream->tellg();
        return new_offset;
    }

    static sf_count_t SfVioRead(void *ptr, sf_count_t count,  void *pData) {
        // read data
        std::istream* stream = static_cast<std::istream*>(pData);
        stream->read(static_cast<char*>(ptr), count);
        sf_count_t bytes_read = stream->gcount() / sizeof(char);
        return bytes_read;
    }

    static sf_count_t SfVioWrite(const void *ptr, sf_count_t count, void *pData) {
        // write
        assert(0); // not implement for input stream
    }

    static sf_count_t SfVioTell(void *pData) {
        // get current location
        std::istream* stream = static_cast<std::istream*>(pData);
        sf_count_t pos = stream->tellg();
        return pos;
    }
}

SoundInfo::SoundInfo(bool enableNormalisation)
        : mEnableNormalisation(enableNormalisation),
          mMode(OPEN_NONE),
          mpSndFile(nullptr),
          mFd(-1),
          mpIns(nullptr) {
}

SoundInfo::~SoundInfo() {
    Close();
}

bool
SoundInfo::Open(const std::string& path, SoundInfo::OpenMode mode) {
    Close();
    mSfInfo.format = 0;  // only requirement by libsndfile sf_open() api
    int sf_mode = SFM_READ;
    int unix_file_mode = O_NONBLOCK | O_CLOEXEC;

    switch(mode) {
        case OPEN_READ_ONLY:
            sf_mode = SFM_READ;
            unix_file_mode = O_RDONLY | unix_file_mode;
            break;
        case OPEN_WRITE_ONLY:
            sf_mode = SFM_WRITE;
            unix_file_mode = O_WRONLY | unix_file_mode;
            break;
        case OPEN_READ_AND_WRITE:
            sf_mode = SFM_RDWR;
            unix_file_mode = O_RDWR | unix_file_mode;
            break;
        default:
            LOG_ERROR("Unknown open mode=%d, assuming read-only\n", mode);
            unix_file_mode = O_RDONLY | unix_file_mode;
            break;
    }
    mFd = open(path.c_str(), unix_file_mode);
    if (mFd < 0) {
        int err = errno;
        LOG_ERROR("open() failed for file=%s, mode=%d, errno=%d\n",
                  path.c_str(), mode, err);
        return false;
    }
    int close_desc = SF_FALSE;  // libsndfile should not close the fd
    // sf_open_fd() is not thread safe in case of error, because it uses static variables.
    // This is the reason the file is opened before and must exist. In case
    // the file is a non-audio file then multi-thread issue might result, but
    // since we dont worry about sf_errno so things should work for now.
    // Look at libsndfile-1.0.26/src/sndfile.c (and search for "Private (static)")
    // for details.
    mpSndFile = sf_open_fd(mFd, sf_mode, &mSfInfo, close_desc);
    if (mpSndFile == nullptr) {
        LOG_ERROR("Cannot open file=%s\n", path.c_str());
        return false;
    }
    // default behaviour is to enable normalisation in libsndfile
    // see http://www.mega-nerd.com/libsndfile/api.html#note1
    if (! mEnableNormalisation) {
        sf_command(mpSndFile, SFC_SET_NORM_FLOAT, nullptr, SF_FALSE);
        sf_command(mpSndFile, SFC_SET_NORM_DOUBLE, nullptr, SF_FALSE);
    }
    return true;
}

bool SoundInfo::OpenStream(std::unique_ptr<std::istream> pIns) {
    Close();

    static SF_VIRTUAL_IO virtual_io = {
            .get_filelen = SfVioGetFileLen,
            .seek = SfVioSeek,
            .read = SfVioRead,
            .write = SfVioWrite,
            .tell = SfVioTell
    };

    mpIns.swap(pIns);

    mSfInfo.format = 0;  // required by the libsndfile api

    // open only in read mode
    mpSndFile = sf_open_virtual(&virtual_io, SFM_READ, &mSfInfo, mpIns.get());
    if (mpSndFile == nullptr) {
        LOG_ERROR("Cannot open stream for reading via libsndfile.\n");
        // dont use sf_error because that is not thread safe
        // LOG_ERROR("error=%s\n", sf_error_number(sf_error(mpSndFile)));
        mpIns.reset(nullptr);
        return false;
    }
    return true;
}

void SoundInfo::Close() {
    if (mpSndFile != nullptr) {
        sf_close(mpSndFile);
        mpSndFile = nullptr;
    }
    if (mFd > 0) {
        close(mFd);
        mFd = -1;
    }
    if (mpIns) {
        mpIns.reset(nullptr);
    }
}

int SoundInfo::GetNumChannels() const {
    return mSfInfo.channels;
}

int SoundInfo::GetSampleRate() const {
    return mSfInfo.samplerate;
}

int64_t SoundInfo::GetNumSamples() const {
    return mSfInfo.frames;
}

int SoundInfo::GetSeekable() const {
    return mSfInfo.seekable;
}

int SoundInfo::GetFormat() const {
    return mSfInfo.format;
}

bool SoundInfo::IsEof() const {
    if (mpIns) {
        // libsndfile do not read once the limit is reached
        // so mpIns->eof() will never return true, hence
        // implementing an alternative (though crude)
        // way of detecting stuff
        //return mpIns->eof();
        sf_count_t pos = mpIns->tellg();
        mpIns->seekg(0, mpIns->end);
        sf_count_t len = mpIns->tellg();
        if (pos == len) {
            return true;
        } else {
            // go back to where you where
            mpIns->seekg(pos, mpIns->beg);
            return false;
        }
    }
    // else TODO
    return false;
}

int64_t SoundInfo::Read(char* pData, int maxElements) {
    short data[maxElements];
    int64_t elements_read = ReadShort(data, maxElements);
    for (int64_t i = 0; i < elements_read; ++i) {
        // retain the most-significant bit during conversion
        pData[i] = static_cast<char>((data[i] >> 8) & 0xff);
    }
    return elements_read;
}

int64_t SoundInfo::Read(unsigned char* pData, int maxElements) {
    short data[maxElements];
    int64_t elements_read = ReadShort(data, maxElements);
    for (int64_t i = 0; i < elements_read; ++i) {
        // retain the most-significant bit during conversion
        pData[i] = static_cast<unsigned char>((data[i] >> 8) & 0xff);
    }
    return elements_read;
}

int64_t SoundInfo::Read(short* pData, int maxElements) {
    int64_t elements_read = ReadShort(pData, maxElements);
    return elements_read;
}

int64_t SoundInfo::Read(int* pData, int maxElements) {
    int64_t elements_read = ReadInt(pData, maxElements);
    return elements_read;
}

int64_t SoundInfo::Read(long* pData, int maxElements) {
    int data[maxElements];
    int64_t elements_read = ReadInt(data, maxElements);
    for (int64_t i = 0; i < elements_read; ++i) {
        pData[i] = data[i];
    }
    return elements_read;
}

int64_t SoundInfo::Read(float* pData, int maxElements) {
    int64_t elements_read = ReadFloat(pData, maxElements);
    return elements_read;
}

int64_t SoundInfo::Read(double* pData, int maxElements) {
    int64_t elements_read = ReadDouble(pData, maxElements);
    return elements_read;
}

bool SoundInfo::Rewind() {
    if (mpSndFile == nullptr) {
        LOG_ERROR("No associated source open, so cannot read anything.\n");
        return false;
    }
    mpIns->clear();  // clear end-of-file or any other internal error state
    // use the sf_seek() method because it will automatically call SfVioSeek()
    // on the stream and skip over the header as well.
    sf_count_t offset = sf_seek(mpSndFile, 0, SEEK_SET);
    if (offset < 0) {
        LOG_ERROR("Cannot seek back to the beginning of the audio file\n");
    }
    return true;
}

int64_t SoundInfo::ReadShort(short* pData, int maxElements) {
    if (mpSndFile == nullptr) {
        LOG_ERROR("No associated source open, so cannot read anything.\n");
        return -1;
    }
    if (maxElements % GetNumChannels() != 0) {
        LOG_ERROR("Can only read elements which are multiple of channels=%d\n",
                  GetNumChannels());
        return -1;
    }
    int64_t num_elements_read = sf_read_short(mpSndFile, pData, maxElements);
    return num_elements_read;
}

int64_t SoundInfo::ReadInt(int* pData, int maxElements) {
    if (mpSndFile == nullptr) {
        LOG_ERROR("No associated source open, so cannot read anything.\n");
        return -1;
    }
    if (maxElements % GetNumChannels() != 0) {
        LOG_ERROR("Can only read elements which are multiple of channels=%d\n",
                  GetNumChannels());
        return -1;
    }
    int64_t num_elements_read = sf_read_int(mpSndFile, pData, maxElements);
    return num_elements_read;
}

int64_t SoundInfo::ReadFloat(float* pData, int maxElements) {
    if (mpSndFile == nullptr) {
        LOG_ERROR("No associated source open, so cannot read anything.\n");
        return -1;
    }
    if (maxElements % GetNumChannels() != 0) {
        LOG_ERROR("Can only read elements which are multiple of channels=%d\n",
                  GetNumChannels());
        return -1;
    }
    int64_t num_elements_read = sf_read_float(mpSndFile, pData, maxElements);
    return num_elements_read;
}

int64_t SoundInfo::ReadDouble(double* pData, int maxElements) {
    if (mpSndFile == nullptr) {
        LOG_ERROR("No associated source open, so cannot read anything.\n");
        return -1;
    }
    if (maxElements % GetNumChannels() != 0) {
        LOG_ERROR("Can only read elements which are multiple of channels=%d\n",
                  GetNumChannels());
        return -1;
    }
    int64_t num_elements_read = sf_read_double(mpSndFile, pData, maxElements);
    return num_elements_read;
}
