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

#include "../common/MemInputStream.h"
#include "../common/Port.h"
#include "../blocks/sources/ComplexStreamSource.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

using namespace sigblocks;

TEST_CASE("Testing math block complex stream source for vector integer", "[complex stream source]") {
    // 10 pairs of (I,Q)
    double data[] = {1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11};
    size_t total_num_data_bytes = sizeof(data);
    size_t bytes_per_field = sizeof(data[0]);
    size_t block_size = total_num_data_bytes / bytes_per_field;

    MemInputStream mem_instream(reinterpret_cast<char*>(&data[0]), total_num_data_bytes);
    std::unique_ptr<std::istream> input_stream(new std::istream(&mem_instream));
    std::shared_ptr<IPort<double> > source(new ComplexStreamSource<double, PORT_TYPE_VECTOR>("complex-1", std::move(input_stream), block_size));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>("archiver-1"));
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = 0;

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 1);
    REQUIRE(archive->GetVectorInfo().front().mTimeTick == timeTick);
    for (size_t i = 0; i < block_size; ++i) {
        expected_result = data[i];
        REQUIRE(archive->GetVectorInfo().front().mData.get()[i] == expected_result);
    }
}

TEST_CASE("Testing math block complex stream source for matrix integer", "[complex stream source]") {
    // 4 pairs of (I,Q)
    double data[] = {1, 2, 2, 3, 3, 4, 4, 5};
    size_t total_num_data_bytes = sizeof(data);
    size_t bytes_per_field = sizeof(data[0]);
    size_t block_size = total_num_data_bytes / bytes_per_field;

    std::vector<int> dims = {2, 2, 1};  // (2 * 2 * 1) == 4 pairs of {I, Q}
    int len = 1;
    for (auto v : dims) {
        len *= v;
    }

    MemInputStream mem_instream(reinterpret_cast<char*>(&data[0]), total_num_data_bytes);
    std::unique_ptr<std::istream> input_stream(new std::istream(&mem_instream));

    std::shared_ptr<IPort<double> > source(new ComplexStreamSource<double, PORT_TYPE_MATRIX>("complex-1", std::move(input_stream), dims));
    std::shared_ptr<IPort<double> > sink(new ArchiverSink<double>("archiver-1"));
    ArchiverSink<double>* archive = dynamic_cast<ArchiverSink<double>*>(sink.get());

    connect(source, sink);

    REQUIRE(source);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    double expected_result = data[0];

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == 1);
    REQUIRE(archive->GetMatrixInfo().front().mTimeTick == timeTick);
    for (int i = 0; i < len; ++i) {
        expected_result = data[i];
        REQUIRE(archive->GetMatrixInfo().front().mData.get()[i] == expected_result);
    }
}