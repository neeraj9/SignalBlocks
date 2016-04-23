// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#include "../common/Port.h"
#include "../blocks/sources/ConstantSource.h"
#include "../blocks/filters/Delay.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

using namespace sigblocks;


TEST_CASE("Testing block delay filter for scalar integer", "[delay filter]") {
    double fixed_value = 1.0;
    int delay_samples = 1;
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_SCALAR>(fixed_value));
    std::shared_ptr<IPort<double> > block(new Delay<double, PORT_TYPE_SCALAR>(delay_samples));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>());
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, connect(block, sink));

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().size() == 0);

    double expected_result = fixed_value;

    timeTick += 1;
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().size() == 1);
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().back().mData == expected_result);
}


TEST_CASE("Testing math block delay filter for vector integer", "[delay filter]") {
    double fixed_value = 1.0;
    int delay_samples = 1;
    int len = 10;
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_VECTOR>(fixed_value, len));
    std::shared_ptr<IPort<double> > block(new Delay<double, PORT_TYPE_VECTOR>(delay_samples));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>());
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, connect(block, sink));

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().empty());

    double expected_result = fixed_value;

    timeTick += 1;
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 1);
    REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetVectorInfo().back().mData.get()[i] == expected_result);
    }
}


TEST_CASE("Testing math block delay filter for matrix integer", "[delay filter]") {
    double fixed_value = 1.0;
    int delay_samples = 1;
    std::vector<int> dims = {4, 2, 1};
    int len = 1;
    for (auto v : dims) {
        len *= v;
    }
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_MATRIX>(fixed_value, dims));
    std::shared_ptr<IPort<double> > block(new Delay<double, PORT_TYPE_MATRIX>(delay_samples));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>());
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, connect(block, sink));

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().empty());
    REQUIRE(archive->GetMatrixInfo().empty());

    double expected_result = fixed_value;

    timeTick += 1;
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == 1);
    REQUIRE(archive->GetMatrixInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetMatrixInfo().back().mData.get()[i] == expected_result);
    }
}