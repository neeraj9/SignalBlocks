// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#include "../common/Port.h"
#include "../blocks/math/Gain.h"
#include "../blocks/sources/ConstantSource.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

using namespace sigblocks;

namespace {
    constexpr int LOOP_COUNT = 10;  // a typical loop count
}


TEST_CASE("Testing math block Gain for scalar integer", "[gain]") {
    int factor = 2;
    int input_value = 4;
    std::shared_ptr<IPort<int> > source(new ConstantSource<int>(input_value));
    std::shared_ptr<IPort<int> > block(new Gain<int>(factor));
    std::shared_ptr<IPort<int> > sink(new ArchiverSink<int>());
    ArchiverSink<int>* archive = dynamic_cast<ArchiverSink<int>*>(sink.get());

    connect(source, connect(block, sink));

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    int expected_result = input_value * factor;

    REQUIRE(archive->GetScalarInfo().size() == 1);
    REQUIRE(archive->GetScalarInfo().front().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().front().mData == expected_result);

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().back().mData == expected_result);
}

TEST_CASE("Testing math block Gain for vector integer", "[gain]") {
    int factor = 2;
    int input_value = 4;
    int len = 10;
    std::shared_ptr<IPort<int> > source(new ConstantSource<int, PORT_TYPE_VECTOR>(input_value, len));
    std::shared_ptr<IPort<int> > block(new Gain<int>(factor));
    std::shared_ptr<IPort<int> > sink(new ArchiverSink<int>());
    ArchiverSink<int>* archive = dynamic_cast<ArchiverSink<int>*>(sink.get());

    connect(source, connect(block, sink));

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    int expected_result = input_value * factor;

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 1);
    REQUIRE(archive->GetVectorInfo().front().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetVectorInfo().front().mData.get()[i] == expected_result);
    }

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetVectorInfo().back().mData.get()[i] == expected_result);
    }
}

TEST_CASE("Testing math block Gain for matrix integer", "[gain]") {
    int factor = 2;
    int input_value = 4;
    std::vector<int> dims = {4, 2, 1};
    int len = 1;
    for (auto v : dims) {
        len *= v;
    }
    std::shared_ptr<IPort<int> > source(new ConstantSource<int, PORT_TYPE_MATRIX>(input_value, dims));
    std::shared_ptr<IPort<int> > block(new Gain<int>(factor));
    std::shared_ptr<IPort<int> > sink(new ArchiverSink<int>());
    ArchiverSink<int>* archive = dynamic_cast<ArchiverSink<int>*>(sink.get());

    connect(source, connect(block, sink));

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    int expected_result = input_value * factor;

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == 1);
    REQUIRE(archive->GetMatrixInfo().front().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetMatrixInfo().front().mData.get()[i] == expected_result);
    }

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetMatrixInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetMatrixInfo().back().mData.get()[i] == expected_result);
    }
}