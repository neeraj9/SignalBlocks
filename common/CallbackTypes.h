// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#ifndef SIGNALBLOCKS_CALLBACKTYPES_H
#define SIGNALBLOCKS_CALLBACKTYPES_H

#include "../common/TimeTick.h"

#include <functional>
#include <string>
#include <tuple>

namespace sigblocks {

    using JsonDataCallbackFuncType =
        std::function<std::tuple<std::string, TimeTick>(bool isFirst, const TimeTick& tick)>;

}

#endif //SIGNALBLOCKS_CALLBACKTYPES_H
