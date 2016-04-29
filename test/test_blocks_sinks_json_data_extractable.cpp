// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license

#include "../common/Port.h"
#include "../blocks/sources/ConstantSource.h"
#include "../blocks/sinks/JsonDataExtractableSink.h"

#include "catch.hpp"

using namespace sigblocks;

TEST_CASE("Testing block json-data-extractable source for scalar integer", "[json-data-extractable source]") {
    double fixed_value = 1.2;
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_SCALAR>("constant-1", fixed_value));
    std::shared_ptr<IPort<double> > sink(new JsonDataExtractableSink<double>("json-data-1"));
    JsonDataExtractableSink<double>* archive = dynamic_cast<JsonDataExtractableSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    std::string expected_result("[{\"name\": \"series-0\", \"data\": [{\"x\": 1, \"y\": 1.2}]}]");

    bool is_first = true;
    std::tuple<std::string, TimeTick> content = archive->GetAsJsonData(is_first, TimeTick());
    std::string json_content(std::move(std::get<0>(content)));
    TimeTick data_tick = std::get<1>(content);

    REQUIRE(data_tick == timeTick);
    REQUIRE(json_content == expected_result);
}

TEST_CASE("Testing math block json-data-extractable source for vector integer", "[json-data-extractable source]") {
    double fixed_value = 1.2;
    int len = 2;
    std::shared_ptr<IPort<double> > source(new ConstantSource<double, PORT_TYPE_VECTOR>("constant-1", fixed_value, len));
    std::shared_ptr<IPort<double> > sink(new JsonDataExtractableSink<double>("json-data-1"));
    JsonDataExtractableSink<double>* archive = dynamic_cast<JsonDataExtractableSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    std::string expected_result(
            "["
                    "{\"name\": \"series-0\", \"data\": [{\"x\": 1, \"y\": 1.2}]},"
                    " {\"name\": \"series-1\", \"data\": [{\"x\": 1, \"y\": 1.2}]}"
            "]");

    bool is_first = true;
    std::tuple<std::string, TimeTick> content = archive->GetAsJsonData(is_first, TimeTick());
    std::string json_content(std::move(std::get<0>(content)));
    TimeTick data_tick = std::get<1>(content);

    REQUIRE(data_tick == timeTick);
    REQUIRE(json_content == expected_result);

    // lets cycle through twice before getting the data, so
    // there will be two samples per vector
    timeTick += 1;
    source->ClockCycle(timeTick);
    timeTick += 1;
    source->ClockCycle(timeTick);

    expected_result = std::string(
            "["
                    "{\"name\": \"series-0\", \"data\": [{\"x\": 2, \"y\": 1.2}, {\"x\": 3, \"y\": 1.2}]},"
                    " {\"name\": \"series-1\", \"data\": [{\"x\": 2, \"y\": 1.2}, {\"x\": 3, \"y\": 1.2}]}"
                    "]");

    is_first = false;
    std::tuple<std::string, TimeTick> content2 = archive->GetAsJsonData(is_first, data_tick);
    std::string json_content2(std::move(std::get<0>(content2)));
    data_tick = std::get<1>(content2);

    REQUIRE(data_tick == timeTick);
    REQUIRE(json_content2 == expected_result);
}

// Matrix support do not exist in JsonDataExtractableSink