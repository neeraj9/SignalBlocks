// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGNALBLOCKS_JSONDATAEXTRACTABLESINK_H
#define SIGNALBLOCKS_JSONDATAEXTRACTABLESINK_H

#include "../../common/Port.h"

#include <deque>
#include <iterator>
#include <mutex>
#include <sstream>
#include <vector>
#include <logging.h>

namespace sigblocks {

    template <class T>
    class DataScalarInfo {
    public:

        DataScalarInfo(const T& data, const TimeTick& timeTick)
                : mData(data),
                  mTimeTick(timeTick) {
        }

        DataScalarInfo(DataScalarInfo&& other)
                : mData(other.mData),
                  mTimeTick(other.mTimeTick) {
        }

        int GetNumElements() const {
            return 1;
        }

        const T& GetData(int index) {
            return mData;
        }

        const TimeTick& GetTimeTick() const {
            return mTimeTick;
        }

        /// dump data from first to last (which include the last iterator as well).
        template <typename M>
        static std::string DataToJson(typename M::iterator first, typename M::iterator last) {
            std::ostringstream out;
            out << "[";
            out << "{";
            out << "\"name\": \"series-0\",";
            out << " \"data\": [";
            typename M::iterator iter = first;
            out << "{\"x\": " << (*iter).mTimeTick.GetValue() << ", \"y\": " << (*iter).mData << "}";
            if (first != last) {
                ++iter;
                for (; iter != last; ++iter) {
                    out << ", {\"x\": " << (*iter).mTimeTick.GetValue() << ", \"y\": " << (*iter).mData << "}";
                }
                out << ", {\"x\": " << (*iter).mTimeTick.GetValue() << ", \"y\": " << (*iter).mData << "}";
            }
            out << "]";
            out << "}";
            out << "]";
            return out.str();
        }

    private:
        T mData;
        TimeTick mTimeTick;
    };

    template <class T>
    class DataVectorInfo {
    public:

        DataVectorInfo(std::unique_ptr<T[]> data, int len, const TimeTick& timeTick)
                : mData(std::move(data)),
                  mLen(len),
                  mTimeTick(timeTick) {
        }

        DataVectorInfo(DataVectorInfo&& other)
                : mData(std::move(other.mData)),
                  mLen(other.mLen),
                  mTimeTick(other.mTimeTick) {
        }

        int GetNumElements() const {
            return mLen;
        }

        const T& GetData(int index) {
            return mData[index];  // no range checking
        }

        const TimeTick& GetTimeTick() const {
            return mTimeTick;
        }

        /// dump data from first to last (which include the last iterator as well).
        template <typename M>
        static std::string DataToJson(typename M::iterator first, typename M::iterator last) {
            std::vector<std::ostringstream> vout(1);
            vout[0] << "{";
            vout[0] << "\"name\": \"series-0\",";
            vout[0] << " \"data\": [";
            typename M::iterator iter = first;
            vout[0] << "{\"x\": " << (*iter).mTimeTick.GetValue() << ", \"y\": " << (*iter).mData[0] << "}";
            for (int i = 1; i < (*iter).mLen; ++i) {
                vout.push_back(std::ostringstream());
                vout[i] << ", {";
                vout[i] << "\"name\": \"series-" << i << "\",";
                vout[i] << " \"data\": [";
                vout[i] << "{\"x\": " << (*iter).mTimeTick.GetValue() << ", \"y\": " << (*iter).mData[i] << "}";
            }
            if (first != last) {
                ++iter;
                bool done = false;
                while (!done) {
                    done = (iter == last);
                    for (int i = 0; i < (*iter).mLen; ++i) {
                        // we can avoid this when all the vectors are trusted to be of the same
                        // size
                        if (vout.size() < static_cast<size_t>(i)) {
                            vout.push_back(std::ostringstream());
                            vout[i] << ", {";
                            vout[i] << "\"name\": \"series-" << i << "\",";
                            vout[i] << " \"data\": [";
                            vout[i] << "{\"x\": " << (*iter).mTimeTick.GetValue() << ", \"y\": " << (*iter).mData[i] << "}";
                        } else {
                            vout[i] << ", {\"x\": " << (*iter).mTimeTick.GetValue() << ", \"y\": " << (*iter).mData[i] << "}";
                        }
                    }
                    ++iter;
                }
            }
            for (auto iter = vout.begin(); iter != vout.end(); ++iter) {
                (*iter) << "]";
                (*iter) << "}";
            }

            // a 2-pass technique to optimize string
            // concatenation
            size_t max_chars = 0;
            for (auto iter = vout.begin(); iter != vout.end(); ++iter) {
                max_chars += iter->tellp();
            }
            std::string json_content;
            json_content.reserve(max_chars + 2);  // include "[" and "]"
            json_content += "[";  // enclose with starting json array marker
            for (auto iter = vout.begin(); iter != vout.end(); ++iter) {
                json_content += iter->str();
            }
            json_content += "]"; // enclose with ending json array marker
            return json_content;
        }

    private:
        std::unique_ptr<T[]> mData;
        int mLen;
        TimeTick mTimeTick;
    };

    /** JsonDataExtractableSink is a sink from which data can be extracted in json format.
     * This block must be used as a sink when data needs to be extracted in
     * json format for archiving or serving via the HttpTcpWebsocketServer().
     *
     * @important do not support Matrix data types at present.
     */
    template<class T>
    class JsonDataExtractableSink
            : public Port<1, 0, T> {
    public:
        // default maximum number of historic data
        static constexpr size_t DEFAULT_MAX_NUM_HISTORIC_DATA = 256;

        JsonDataExtractableSink(size_t maxNumHistoricData = DEFAULT_MAX_NUM_HISTORIC_DATA)
                : mMutex(),  // unlocked state
                  mMaxNumHistoricData(maxNumHistoricData),
                  mSingleShotNumDataDeleted(maxNumHistoricData / 4) {
            if (mSingleShotNumDataDeleted == 0) {
                mSingleShotNumDataDeleted = 1;  // cannot be 0
            }
        }

        std::tuple<std::string, TimeTick> GetAsJsonData(bool isFirst, const TimeTick& tick) {
            LOG_DEBUG("is_first=%s, tick=%lu\n", isFirst ? "true" : "false", tick.GetValue());
            std::string json_output;
            TimeTick updated_tick = tick;
            if (isFirst) {
                // take ownership till the end of the block
                std::lock_guard<std::mutex> lock(mMutex);
                // first try scalar storage and if not then check for vector
                // Important: it is assumed that the previous block would
                // be consistent and use either scalar or vector always.
                if (!mScalarStorage.empty()) {
                    auto first = mScalarStorage.begin();
                    auto last = mScalarStorage.end() - 1;
                    std::string tmp =
                            DataScalarInfo<T>::template DataToJson<std::deque<DataScalarInfo<T> > >(first, last);
                    json_output.swap(tmp);
                    updated_tick = last->GetTimeTick();
                } else if (!mVectorStorage.empty()) {
                    auto first = mVectorStorage.begin();
                    auto last = mVectorStorage.end() - 1;
                    std::string tmp =
                            DataVectorInfo<T>::template DataToJson<std::deque<DataVectorInfo<T> > >(first, last);
                    json_output.swap(tmp);
                    updated_tick = last->GetTimeTick();
                }
            } else {
                std::lock_guard<std::mutex> lock(mMutex);
                if (!mScalarStorage.empty()) {
                    // its better to archive last useful iterator
                    // because addition to mScalarStorage is not protected by
                    // mutex (see Process()), so although insertion to
                    // deque do not invalidate the previous iterators but
                    // race condition can potentially happen (in multi-threaded
                    // environment). Hence lets avoid it to be safe in a
                    // multi-threaded enviroment without loss of performance.
                    auto first = mScalarStorage.begin();
                    auto last = mScalarStorage.end() - 1;
                    for (; first != last; ++first) {
                        if (first->GetTimeTick().IsGreater(tick)) {
                            break;
                        }
                    }
                    if (first == last) {
                        if (!first->GetTimeTick().IsGreater(tick)) {
                            return std::make_tuple(std::string(), TimeTick());
                        }
                    }
                    std::string tmp =
                            DataScalarInfo<T>::template DataToJson<std::deque<DataScalarInfo<T> > >(first, last);
                    json_output.swap(tmp);
                    updated_tick = last->GetTimeTick();
                } else if (!mVectorStorage.empty()) {
                    // The same protection applies here as for the mScalarStorage.
                    // See above for details.
                    auto first = mVectorStorage.begin();
                    auto last = mVectorStorage.end() - 1;
                    for (; first != last; ++first) {
                        if (first->GetTimeTick().IsGreater(tick)) {
                            break;
                        }
                    }
                    if (first == last) {
                        if (!first->GetTimeTick().IsGreater(tick)) {
                            return std::make_tuple(std::string(), TimeTick());
                        }
                    }
                    std::string tmp =
                            DataVectorInfo<T>::template DataToJson<std::deque<DataVectorInfo<T> > >(first, last);
                    json_output.swap(tmp);
                    updated_tick = last->GetTimeTick();
                }
            }
            LOG_DEBUG("updated_tick=%lu\n", updated_tick.GetValue());
            return std::make_tuple(std::move(json_output), updated_tick);
        };

    public: // Port interface
        void Process(int sourceIndex, const T& data, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            if (mScalarStorage.size() >= mMaxNumHistoricData) {
                std::lock_guard<std::mutex> lock(mMutex);
                // Delete in bulk to avoid push/pop sequence once the limit
                // is reached, which unnecessarily degrades performance.
                // It does looses data when the frequency of data insertion
                // do not match that while pulling out the Json formatted,
                // but that is fine in-lieu of the performance gain.
                // As a fix try to call GetAsJsonData() as frequent
                // as the data is inserted here.
                for (size_t i = 0; i < mSingleShotNumDataDeleted; ++i) {
                    mScalarStorage.pop_front();  // get rid of oldest data
                }
            }
            // adding to the deque do not alter existing elements
            mScalarStorage.push_back(DataScalarInfo<T>(data, startTime));
        }

        void Process(
                int sourceIndex, std::unique_ptr<T[]> data, int len, const TimeTick& startTime) {
            assert(sourceIndex == 0);
            if (mVectorStorage.size() >= mMaxNumHistoricData) {
                std::lock_guard<std::mutex> lock(mMutex);
                mVectorStorage.pop_front();  // get rid of oldest data
            }
            mVectorStorage.push_back(DataVectorInfo<T>(std::move(data), len, startTime));
        }

        void ProcessMatrix(
                int sourceIndex, std::unique_ptr<T[]> data, const std::vector<int>& dims, const TimeTick& startTime) {
            assert(0);  // do not support this at present
        }

    private:
        std::mutex mMutex;
        size_t mMaxNumHistoricData;
        size_t mSingleShotNumDataDeleted;  ///< avoid repeated pop from the queue
        std::deque<DataScalarInfo<T> > mScalarStorage;
        std::deque<DataVectorInfo<T> > mVectorStorage;
    };
}

#endif //SIGNALBLOCKS_JSONDATAEXTRACTABLESINK_H
