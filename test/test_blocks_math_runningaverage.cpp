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

#include "../common/Port.h"
#include "../blocks/math/RunningAverage.h"
#include "../blocks/sources/ConstantSource.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

#include <cmath>

using namespace signalblocks;

namespace {
    constexpr int LOOP_COUNT = 10;  // a typical loop count
    constexpr double DELTA_THRESHOLD = 1e-10;

    // real numbers may not exactly match
    double RoughlyEqual(double a, double b) {
        double delta = ::fabs(a - b);
        return (delta < DELTA_THRESHOLD);
    }
}


TEST_CASE("Testing block running average for scalar integer", "[running average]") {
    // no ctor args required
    double input_value = M_PI;
    std::shared_ptr<IPort<double> > source(new ConstantSource<double>("constant-1", input_value));
    std::shared_ptr<IPort<double> > block(new RunningAverage<double>("running average-block-1"));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>("archive-1"));
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, connect(block, sink));

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = input_value;

    REQUIRE(archive->GetScalarInfo().size() == 1);
    REQUIRE(archive->GetScalarInfo().front().mTimeTick == timeTick);
    REQUIRE(RoughlyEqual(archive->GetScalarInfo().front().mData, expected_result));

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(RoughlyEqual(archive->GetScalarInfo().back().mData, expected_result));
}

TEST_CASE("Testing math block running average for vector integer", "[running average]") {
    double input_value = M_PI;
    int len = 10;
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_VECTOR>("constant-1", input_value, len));
    std::shared_ptr<IPort<double> > block(new RunningAverage<double>("running average-1"));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>("archive-1"));
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, connect(block, sink));

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = input_value;

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 1);
    REQUIRE(archive->GetVectorInfo().front().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(RoughlyEqual(archive->GetVectorInfo().front().mData.get()[i], expected_result));
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
        REQUIRE(RoughlyEqual(archive->GetVectorInfo().back().mData.get()[i], expected_result));
    }
}

TEST_CASE("Testing math block running average for matrix integer", "[running average]") {
    // no ctor args required
    double input_value = M_PI;
    std::vector<int> dims = {4, 2, 1};
    int len = 1;
    for (auto v : dims) {
        len *= v;
    }
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_MATRIX>("constant-1", input_value, dims));
    std::shared_ptr<IPort<double> > block(new RunningAverage<double>("running average-1"));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>("archiver-1"));
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, connect(block, sink));

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = input_value;

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == 1);
    REQUIRE(archive->GetMatrixInfo().front().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(RoughlyEqual(archive->GetMatrixInfo().front().mData.get()[i], expected_result));
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
        REQUIRE(RoughlyEqual(archive->GetMatrixInfo().back().mData.get()[i], expected_result));
    }
}

