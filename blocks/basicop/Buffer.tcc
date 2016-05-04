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

#ifndef SIGNALBLOCKS_BUFFER_TCC
#define SIGNALBLOCKS_BUFFER_TCC

#include <algorithm>
#include <cassert>
#include <vector>

namespace signalblocks {

    template<class T>
    Buffer<T>::Buffer(std::string name, int fieldsToBuffer)
            : Port<1, 1, T>(std::move(name), "A buffer block."),
              mFieldsToBuffer(fieldsToBuffer),
              mNumFields(0),
              mData(new T[fieldsToBuffer]) {  // pre-allocate space
    }


    template<class T>
    void Buffer<T>::Process(int sourceIndex, const T& data, const TimeTick& startTime) {
        assert(sourceIndex == 0); // XXX change to an assertion library.

        mData.get()[mNumFields++] = data;
        if (mNumFields >= mFieldsToBuffer) {
            this->LeakData(0, std::move(mData), mNumFields, startTime);
            if (mData.get() == 0) // this is always true at present
            {
                mData.reset(new T[mFieldsToBuffer]);
            }
            mNumFields = 0;
        }
    }

    template<class T>
    void Buffer<T>::Process(
            int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
        assert(sourceIndex == 0); // XXX change to an assertion library.

        int offset = 0;
        int fields_to_copy = ((mNumFields + len - mFieldsToBuffer) >= 0) ? (mFieldsToBuffer - mNumFields) : len;
        while (fields_to_copy > 0) {
            // fill mData with rest of the contents
            // TODO if type T is a non-native type then the following will result in shallow copy
            // which is an issue. Note that at present this block do not support such types
            // but in case it does then change this implementation
            std::copy(data.get() + offset,
                      data.get() + fields_to_copy,
                      mData.get() + mNumFields);
            mNumFields += fields_to_copy;
            if (mNumFields >= mFieldsToBuffer) {
                this->LeakData(0, std::move(mData), mNumFields, startTime);
                if (mData.get() == 0) // this is always true at present
                {
                    mData.reset(new T[mFieldsToBuffer]);
                }
                mNumFields = 0;
            }
            offset += fields_to_copy;
            fields_to_copy = ((mNumFields + len - offset - mFieldsToBuffer) >= 0) ? (mFieldsToBuffer - mNumFields) : (len -
                                                                                                                      offset);
        }
    }

    template<class T>
    void Buffer<T>::ProcessMatrix(int sourceIndex,
                               std::unique_ptr<T[]> data,
                               const std::vector<int>& dims,
                               const TimeTick& startTime) {
        assert(sourceIndex == 0); // XXX change to an assertion library.
        // validate the dims in accordance with mFieldsToBuffer
        // and compute the overall length as well
        int len = dims[0];
        // The input data received must be such that mFieldsToBuffer contains
        // a subset of dims, that is mFieldsToBuffer must be equal to
        // data size of integral multiple of some (or all) the dimensions.
        // For example: for a mFieldsToBuffer = 4 is valid for
        //     dims = {2, 2, 10, 4} (because 2 x 2 = 4) or
        //     dims = {4, 20, 4, 3} (because 4 = 4)
        bool is_input_valid = false;
        if (len == mFieldsToBuffer) {
            is_input_valid = true;
        }
        for (size_t i = 1; i < dims.size(); ++i) {
            len *= dims[i];
            if (len == mFieldsToBuffer) {
                is_input_valid = true;
            }
        }
        assert(is_input_valid);

        // TODO this code is duplicate of Process() for vector and should be extracted out
        // into common code for reuse.
        int offset = 0;
        int fields_to_copy = ((mNumFields + len - mFieldsToBuffer) >= 0) ? (mFieldsToBuffer - mNumFields) : len;
        while (fields_to_copy > 0) {
            // fill mData with rest of the contents
            std::copy(data.get() + offset,
                      data.get() + fields_to_copy,
                      mData.get() + mNumFields);
            mNumFields += fields_to_copy;
            if (mNumFields >= mFieldsToBuffer) {
                this->LeakData(0, std::move(mData), mNumFields, startTime);
                if (mData.get() == 0) // this is always true at present
                {
                    mData.reset(new T[mFieldsToBuffer]);
                }
                mNumFields = 0;
            }
            offset += fields_to_copy;
            fields_to_copy = ((mNumFields + len - offset - mFieldsToBuffer) >= 0) ? (mFieldsToBuffer - mNumFields) : (len -
                                                                                                                      offset);
        }
    }
}

#endif // SIGNALBLOCKS_BUFFER_TCC
