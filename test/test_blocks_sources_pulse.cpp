// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#include "../common/Port.h"
#include "../blocks/sources/PulseSource.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

using namespace sigblocks;

namespace {
    constexpr int LOOP_COUNT = 10;  // a typical loop count
}



TEST_CASE("Testing block pulse source for scalar integer", "[pulse source]") {
    double period_offset = 0.25;
    int total_period_samples = 10;
    double high_ratio = 0.5;  // 50% high and 50% low
    double high_value = 20.5;
    double low_value = -1.0;
    std::shared_ptr<IPort<double> > source(new PulseSource<double, PORT_TYPE_SCALAR>(
            period_offset, total_period_samples, high_ratio, high_value, low_value));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>());
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    int period_sample = static_cast<int>(total_period_samples * period_offset);
    int num_high_samples = static_cast<int>(total_period_samples * high_ratio);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = high_value;
    if (period_sample > num_high_samples) {
        expected_result = low_value;
    }

    REQUIRE(archive->GetScalarInfo().size() == 1);
    REQUIRE(archive->GetScalarInfo().front().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().front().mData == expected_result);

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
        period_sample = (period_sample + 1) % total_period_samples;
        if (period_sample > num_high_samples) {
            expected_result = low_value;
        } else {
            expected_result = high_value;
        }
    }

    REQUIRE(archive->GetScalarInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().back().mData == expected_result);
}

TEST_CASE("Testing math block pulse source for vector integer", "[pulse source]") {
    double period_offset = 0.25;
    int total_period_samples = 10;
    double high_ratio = 0.5;  // 50% high and 50% low
    double high_value = 20.5;
    double low_value = -1.0;    int len = 10;
    std::shared_ptr<IPort<double> > source(new PulseSource<double, PORT_TYPE_VECTOR>(
            period_offset, total_period_samples, high_ratio, high_value, low_value, len));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>());
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    int period_sample = static_cast<int>(total_period_samples * period_offset);
    int num_high_samples = static_cast<int>(total_period_samples * high_ratio);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = high_value;
    if (period_sample > num_high_samples) {
        expected_result = low_value;
    }

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
        period_sample = (period_sample + 1) % total_period_samples;
        if (period_sample > num_high_samples) {
            expected_result = low_value;
        } else {
            expected_result = high_value;
        }
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetVectorInfo().back().mData.get()[i] == expected_result);
    }
}

TEST_CASE("Testing math block pulse source for matrix integer", "[pulse source]") {
    double period_offset = 0.25;
    int total_period_samples = 10;
    double high_ratio = 0.5;  // 50% high and 50% low
    double high_value = 20.5;
    double low_value = -1.0;
    std::vector<int> dims = {4, 2, 1};
    int len = 1;
    for (auto v : dims) {
        len *= v;
    }
    std::shared_ptr<IPort<double> > source(new PulseSource<double, PORT_TYPE_MATRIX>(
            period_offset, total_period_samples, high_ratio, high_value, low_value, dims));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>());
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    int period_sample = static_cast<int>(total_period_samples * period_offset);
    int num_high_samples = static_cast<int>(total_period_samples * high_ratio);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = high_value;
    if (period_sample > num_high_samples) {
        expected_result = low_value;
    }

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
        period_sample = (period_sample + 1) % total_period_samples;
        if (period_sample > num_high_samples) {
            expected_result = low_value;
        } else {
            expected_result = high_value;
        }
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetMatrixInfo().back().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        REQUIRE(archive->GetMatrixInfo().back().mData.get()[i] == expected_result);
    }
}