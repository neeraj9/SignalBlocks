// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#ifndef SIGNALBLOCKS_ORDINALCONVERTER_H
#define SIGNALBLOCKS_ORDINALCONVERTER_H

#include "../../common/MixedPort.h"

#include <tuple>
#include <unordered_map>

namespace sigblocks {
    class OrdinalConverter
            : public MixedPort<1, 1, std::string, unsigned long> {

    public:
        OrdinalConverter();

        OrdinalConverter(std::unordered_map<std::string, unsigned long> initDict,
                         unsigned long nextOrdinalValue);

        std::tuple<unsigned long, bool> GetOrdinalValue(const std::string& key) const;
        unsigned long GetNextOrdinalValue() const;

        std::unordered_map<std::string, unsigned long> ExtractDictionary();

    protected: // Override MixedPort interfaces for Sink
        virtual void Process(int sourceIndex, const std::string& data, const TimeTick& startTime);

        virtual void Process(
                int sourceIndex, std::unique_ptr<std::string[]> data, int len, const TimeTick& startTime);

    private:
        std::unordered_map<std::string, unsigned long> mDictionary;
        unsigned long mNextOrdinalValue;
    };
}


#endif //SIGNALBLOCKS_ORDINALCONVERTER_H
