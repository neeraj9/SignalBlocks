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
#include "../blocks/sources/RandomSource.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

using namespace sigblocks;


TEST_CASE("Testing block random source for scalar integer", "[random source]") {
    // no ctor args required
    std::shared_ptr<IPort<double> > source(new RandomSource<double, PORT_TYPE_SCALAR>("random-1"));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>("archiver-1"));
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double unexpected_result = archive->GetScalarInfo().front().mData;

    REQUIRE(archive->GetScalarInfo().size() == 1);
    REQUIRE(archive->GetScalarInfo().front().mTimeTick == timeTick);

    timeTick += 1;
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().size() == (1 + 1));
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().back().mData != unexpected_result);
}

TEST_CASE("Testing math block random source for vector integer", "[random source]") {
    // no ctor args required
    int len = 10;
    std::shared_ptr<IPort<double> > source(new RandomSource<double, PORT_TYPE_VECTOR>("random-1", len));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>("archiver-1"));
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double unexpected_result = archive->GetVectorInfo().front().mData.get()[0];

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 1);
    REQUIRE(archive->GetVectorInfo().front().mTimeTick == timeTick);

    timeTick += 1;
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == (1 + 1));
    REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetVectorInfo().back().mData.get()[i] != unexpected_result);
    }
}

TEST_CASE("Testing math block random source for matrix integer", "[random source]") {
    // no ctor args required
    std::vector<int> dims = {4, 2, 1};
    int len = 1;
    for (auto v : dims) {
        len *= v;
    }
    std::shared_ptr<IPort<double> > source(new RandomSource<double, PORT_TYPE_MATRIX>("random-1", dims));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>("archiver-1"));
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double unexpected_result = archive->GetMatrixInfo().front().mData.get()[0];

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == 1);
    REQUIRE(archive->GetMatrixInfo().front().mTimeTick == timeTick);

    timeTick += 1;
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == (1 + 1));
    REQUIRE(archive->GetMatrixInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetMatrixInfo().back().mData.get()[i] != unexpected_result);
    }
}