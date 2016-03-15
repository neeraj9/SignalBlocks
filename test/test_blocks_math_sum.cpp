// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#include "../common/Port.h"
#include "../blocks/math/Sum.h"
#include "../blocks/sources/ConstantSource.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

using namespace sigblocks;

namespace {
    constexpr int LOOP_COUNT = 10;  // a typical loop count
}


TEST_CASE("Testing math block Sum for scalar integer", "[sum]") {
    int a = 4;
    int b = 17;
    std::shared_ptr<IPort<int> > source1(new ConstantSource<int>(a));
    std::shared_ptr<IPort<int> > source2(new ConstantSource<int>(b));
    std::shared_ptr<IPort<int> > block(new Sum<2, int>());
    std::shared_ptr<IPort<int> > sink(new ArchiverSink<int>());
    ArchiverSink<int>* archive = dynamic_cast<ArchiverSink<int>*>(sink.get());

    connect(source1, connect(block, sink));
    connect(source2, block, 0, 1);

    REQUIRE(source1);
    REQUIRE(source2);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    LOG_DEBUG("Before first tick\n");
    TimeTick timeTick(0);
    source1->ClockCycle(timeTick);
    LOG_DEBUG("clock cycle=%lu, 0\n", timeTick.GetValue());
    source2->ClockCycle(timeTick);
    LOG_DEBUG("clock cycle=%lu, 1\n", timeTick.GetValue());

    int expected_result = a + b;

    REQUIRE(archive->GetScalarInfo().size() == 1);
    REQUIRE(archive->GetScalarInfo().front().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().front().mData == expected_result);

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        LOG_DEBUG("Before clock cycle=%lu\n", timeTick.GetValue());
        source1->ClockCycle(timeTick);
        LOG_DEBUG("clock cycle=%lu, 0\n", timeTick.GetValue());
        source2->ClockCycle(timeTick);
        LOG_DEBUG("clock cycle=%lu, 1\n", timeTick.GetValue());
    }

    REQUIRE(archive->GetScalarInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().back().mData == expected_result);
}

TEST_CASE("Testing math block Sum for vector integer", "[sum]") {
    int a = 4;
    int b = 17;
    int len = 10;
    std::shared_ptr<IPort<int> > source1(new ConstantSource<int, PORT_TYPE_VECTOR>(a, len));
    std::shared_ptr<IPort<int> > source2(new ConstantSource<int, PORT_TYPE_VECTOR>(b, len));
    std::shared_ptr<IPort<int> > block(new Sum<2, int>());
    std::shared_ptr<IPort<int> > sink(new ArchiverSink<int>());
    ArchiverSink<int>* archive = dynamic_cast<ArchiverSink<int>*>(sink.get());

    connect(source1, connect(block, sink));
    connect(source2, block, 0, 1);

    REQUIRE(source1);
    REQUIRE(source2);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(0);
    source1->ClockCycle(timeTick);
    source2->ClockCycle(timeTick);

    int expected_result = a + b;

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 1);
    REQUIRE(archive->GetVectorInfo().front().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetVectorInfo().front().mData.get()[i] == expected_result);
    }

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source1->ClockCycle(timeTick);
        source2->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetVectorInfo().back().mData.get()[i] == expected_result);
    }
}

TEST_CASE("Testing math block Sum for matrix integer", "[sum]") {
    int a = 4;
    int b = 17;
    std::vector<int> dims = {4, 2, 1};
    int len = 1;
    for (auto v : dims) {
        len *= v;
    }
    std::shared_ptr<IPort<int> > source1(new ConstantSource<int, PORT_TYPE_MATRIX>(a, dims));
    std::shared_ptr<IPort<int> > source2(new ConstantSource<int, PORT_TYPE_MATRIX>(b, dims));
    std::shared_ptr<IPort<int> > block(new Sum<2, int>());
    std::shared_ptr<IPort<int> > sink(new ArchiverSink<int>());
    ArchiverSink<int>* archive = dynamic_cast<ArchiverSink<int>*>(sink.get());

    connect(source1, connect(block, sink));
    connect(source2, block, 0, 1);

    REQUIRE(source1);
    REQUIRE(source2);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(0);
    source1->ClockCycle(timeTick);
    source2->ClockCycle(timeTick);

    int expected_result = a + b;

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
        source1->ClockCycle(timeTick);
        source2->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetMatrixInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetMatrixInfo().back().mData.get()[i] == expected_result);
    }
}