// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#include "../common/Port.h"
#include "../blocks/sources/ConstantSource.h"
#include "../blocks/basicop/Duplicator.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

using namespace sigblocks;

namespace {
    constexpr int LOOP_COUNT = 10;  // a typical loop count
}

TEST_CASE("Testing basicop duplicator for scalar integer", "[duplicator]") {
    double fixed_value = 0.0;
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_SCALAR>("constant-1", fixed_value));
    std::shared_ptr<IPort<double> > block(new Duplicator<2, double>("duplicator"));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>("archiver-1"));
    std::shared_ptr<IPort<double> > sink2(new ArchiverSink<double>("archiver-2"));

    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());
    ArchiverSink<double>* archive2 = dynamic_cast<ArchiverSink<double>*>(sink2.get());

    connect(source, connect(connect(block, sink), sink2, 1, 0));

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(sink2);
    REQUIRE(archive);
    REQUIRE(archive2);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = fixed_value;

    REQUIRE(archive->GetScalarInfo().size() == 1);
    REQUIRE(archive->GetScalarInfo().front().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().front().mData == expected_result);

    REQUIRE(archive2->GetScalarInfo().size() == 1);
    REQUIRE(archive2->GetScalarInfo().front().mTimeTick == timeTick);
    REQUIRE(archive2->GetScalarInfo().front().mData == expected_result);

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().back().mData == expected_result);

    REQUIRE(archive2->GetScalarInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive2->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive2->GetScalarInfo().back().mData == expected_result);
}

TEST_CASE("Testing basicop duplicator for vector integer", "[duplicator]") {
    double fixed_value = 0.0;
    int len = 10;
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_VECTOR>("constant-1", fixed_value, len));
    std::shared_ptr<IPort<double> > block(new Duplicator<2, double>("duplicator"));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>("archiver-1"));
    std::shared_ptr<IPort<double> > sink2(new ArchiverSink<double>("archiver-2"));

    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());
    ArchiverSink<double>* archive2 = dynamic_cast<ArchiverSink<double>*>(sink2.get());

    connect(source, connect(connect(block, sink), sink2, 1, 0));

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(sink2);
    REQUIRE(archive);
    REQUIRE(archive2);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = fixed_value;

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 1);
    REQUIRE(archive->GetVectorInfo().front().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetVectorInfo().front().mData.get()[i] == expected_result);
    }

    REQUIRE(archive2->GetScalarInfo().empty());
    REQUIRE(archive2->GetVectorInfo().size() == 1);
    REQUIRE(archive2->GetVectorInfo().front().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive2->GetVectorInfo().front().mData.get()[i] == expected_result);
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

    REQUIRE(archive2->GetScalarInfo().empty());
    REQUIRE(archive2->GetVectorInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive2->GetVectorInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive2->GetVectorInfo().back().mData.get()[i] == expected_result);
    }
}

TEST_CASE("Testing basicop duplicator for matrix integer", "[duplicator]") {
    double fixed_value = 0.0;
    std::vector<int> dims = {4, 2, 1};
    int len = 1;
    for (auto v : dims) {
        len *= v;
    }
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_MATRIX>("constant-1", fixed_value, dims));
    std::shared_ptr<IPort<double> > block(new Duplicator<2, double>("duplicator"));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>("archiver-1"));
    std::shared_ptr<IPort<double> > sink2(new ArchiverSink<double>("archiver-2"));

    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());
    ArchiverSink<double>* archive2 = dynamic_cast<ArchiverSink<double>*>(sink2.get());

    connect(source, connect(connect(block, sink), sink2, 1, 0));

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(sink2);
    REQUIRE(archive);
    REQUIRE(archive2);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = fixed_value;

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == 1);
    REQUIRE(archive->GetMatrixInfo().front().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetMatrixInfo().front().mData.get()[i] == expected_result);
    }

    REQUIRE(archive2->GetScalarInfo().empty());
    REQUIRE(archive2->GetVectorInfo().empty());
    REQUIRE(archive2->GetMatrixInfo().size() == 1);
    REQUIRE(archive2->GetMatrixInfo().front().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive2->GetMatrixInfo().front().mData.get()[i] == expected_result);
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

    REQUIRE(archive2->GetScalarInfo().empty());
    REQUIRE(archive2->GetMatrixInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive2->GetMatrixInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive2->GetMatrixInfo().back().mData.get()[i] == expected_result);
    }
}
