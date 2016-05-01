// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#include "../common/Port.h"
#include "../blocks/sources/ConstantSource.h"
#include "../blocks/sinks/StdoutSink.h"

#include "catch.hpp"

using namespace sigblocks;

namespace {
    constexpr int LOOP_COUNT = 10;  // a typical loop count
}

TEST_CASE("Testing block stdout sink for scalar integer", "[stdout sink]") {
    double fixed_value = 0.0;
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_SCALAR>("constant-1", fixed_value));
    std::shared_ptr<IPort<double> > sink(new StdoutSink<double>("standard-out"));

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }
}

TEST_CASE("Testing math block stdout sink for vector integer", "[stdout sink]") {
    double fixed_value = 0.0;
    int len = 10;
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_VECTOR>("constant-1", fixed_value, len));
    std::shared_ptr<IPort<double> > sink(new StdoutSink<double>("standard-out"));

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }
}

TEST_CASE("Testing math block stdout sink for matrix integer", "[stdout sink]") {
    double fixed_value = 0.0;
    std::vector<int> dims = {4, 2, 1};
    int len = 1;
    for (auto v : dims) {
        len *= v;
    }
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_MATRIX>("constant-1", fixed_value, dims));
    std::shared_ptr<IPort<double> > sink(new StdoutSink<double>("standard-out"));

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }
}
