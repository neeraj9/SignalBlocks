// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#include "../common/Port.h"
#include "../blocks/sources/LinearSource.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

using namespace sigblocks;

namespace {
    constexpr int LOOP_COUNT = 10;  // a typical loop count
}



TEST_CASE("Testing block linear source for scalar integer", "[linear source]") {
    // no ctor args required
    double initial_value = 0.0;
    double increment = 1.5;
    std::shared_ptr<IPort<double> > source(new LinearSource<double, PORT_TYPE_SCALAR>(initial_value, increment));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>());
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = initial_value;

    REQUIRE(archive->GetScalarInfo().size() == 1);
    REQUIRE(archive->GetScalarInfo().front().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().front().mData == expected_result);

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
        expected_result += increment;
    }

    REQUIRE(archive->GetScalarInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().back().mData == expected_result);
}

TEST_CASE("Testing math block linear source for vector integer", "[linear source]") {
    // no ctor args required
    double initial_value = 0.0;
    double increment = 1.5;
    int len = 10;
    std::shared_ptr<IPort<double> > source(new LinearSource<double, PORT_TYPE_VECTOR>(initial_value, increment, len));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>());
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = initial_value;

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
        expected_result += increment;
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetVectorInfo().back().mData.get()[i] == expected_result);
    }
}

TEST_CASE("Testing math block linear source for matrix integer", "[linear source]") {
    // no ctor args required
    double initial_value = 0.0;
    double increment = 1.5;
    std::vector<int> dims = {4, 2, 1};
    int len = 1;
    for (auto v : dims) {
        len *= v;
    }
    std::shared_ptr<IPort<double> > source(new LinearSource<double, PORT_TYPE_MATRIX>(initial_value, increment, dims));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>());
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = initial_value;

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
        expected_result += increment;
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetMatrixInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetMatrixInfo().back().mData.get()[i] == expected_result);
    }
}