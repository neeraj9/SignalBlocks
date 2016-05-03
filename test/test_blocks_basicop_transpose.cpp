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
#include "../blocks/basicop/Splitter.h"
#include "../blocks/basicop/Transpose.h"
#include "../blocks/converters/ToMatrixConverter.h"
#include "../blocks/sources/IstreamSource.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

using namespace sigblocks;

namespace {
    constexpr int LOOP_COUNT = 10;  // a typical loop count
}

using BaseDataType = uint8_t;

TEST_CASE("Testing block transpose for scalar integer", "[transpose]") {
    size_t total_num_data_bytes = 4 * 3;
    BaseDataType buffer[total_num_data_bytes];
    for (BaseDataType i = 0; i < total_num_data_bytes; ++i) {
        buffer[i] = i;
    }

    MemInputStream mem_instream(reinterpret_cast<char*>(&buffer[0]), total_num_data_bytes * sizeof(BaseDataType));
    std::unique_ptr<std::istream> input_stream(new std::istream(&mem_instream));

    std::shared_ptr<IPort<BaseDataType> > source(new IstreamSource<BaseDataType>(
            "istream-source-1",
            1,  // generate one element at a time but as vector output
            std::move(input_stream)));
    // Using the splitter as vector to scalar converter here. Note
    // that the input to the splitter block is a vector or size 1,
    // because IstreamSource can only generate vector output.
    // This is the reason why we require a splitter block to convert to
    // scalar output.
    std::shared_ptr<IPort<BaseDataType> > toscalar(new Splitter<1, BaseDataType>("splitter-1", 0));
    std::shared_ptr<IPort<BaseDataType> > block(new Transpose<BaseDataType>("transport-1"));
    std::shared_ptr<IPort<BaseDataType> > sink(new ArchiverSink<BaseDataType>("archiver-1"));
    ArchiverSink<BaseDataType>* archive = dynamic_cast<ArchiverSink<BaseDataType>*>(sink.get());

    connect(source, connect(toscalar, connect(block, sink)));

    REQUIRE(source);
    REQUIRE(toscalar);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    size_t offset = 0;
    BaseDataType expected_result = buffer[offset];

    REQUIRE(archive->GetScalarInfo().size() == 1);
    REQUIRE(archive->GetScalarInfo().front().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().front().mData == expected_result);

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
        offset = (offset + 1) % total_num_data_bytes;
        expected_result = buffer[offset];
    }

    REQUIRE(archive->GetScalarInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetScalarInfo().back().mData == expected_result);
}

TEST_CASE("Testing math block transpose for vector integer", "[transpose]") {
    size_t total_num_data_bytes = 12;
    BaseDataType buffer[total_num_data_bytes];
    for (BaseDataType i = 0; i < total_num_data_bytes; ++i) {
        buffer[i] = i;
    }

    MemInputStream mem_instream(reinterpret_cast<char*>(&buffer[0]), total_num_data_bytes * sizeof(BaseDataType));
    std::unique_ptr<std::istream> input_stream(new std::istream(&mem_instream));

    std::shared_ptr<IPort<BaseDataType> > source(new IstreamSource<BaseDataType>(
            "istream-1",
            static_cast<int>(total_num_data_bytes),
            std::move(input_stream)));
    std::shared_ptr<IPort<BaseDataType> > block(new Transpose<BaseDataType>("transpose-1"));
    std::shared_ptr<IPort<BaseDataType> > sink(new ArchiverSink<BaseDataType>("archiver-1"));
    ArchiverSink<BaseDataType>* archive = dynamic_cast<ArchiverSink<BaseDataType>*>(sink.get());

    connect(source, connect(block, sink));

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 1);
    REQUIRE(archive->GetVectorInfo().front().mTimeTick == timeTick);
    REQUIRE(archive->GetVectorInfo().front().mLen == total_num_data_bytes);
    for (size_t i = 0; i < total_num_data_bytes; ++i) {
        REQUIRE(archive->GetVectorInfo().front().mData.get()[i] == buffer[i]);
    }

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetVectorInfo().back().mLen == total_num_data_bytes);
    for (size_t i = 0; i < total_num_data_bytes; ++i) {
        REQUIRE(archive->GetVectorInfo().back().mData.get()[i] == buffer[i]);
    }
}

TEST_CASE("Testing math block transpose for matrix integer", "[transpose]") {
    std::vector<int> dims = {4, 3};
    size_t total_num_data_bytes = static_cast<size_t>(dims[0] * dims[1]);
    BaseDataType buffer[total_num_data_bytes];
    for (size_t i = 0; i < total_num_data_bytes; ++i) {
        buffer[i] = static_cast<BaseDataType>(i);
    }

    MemInputStream mem_instream(reinterpret_cast<char*>(&buffer[0]), total_num_data_bytes * sizeof(BaseDataType));
    std::unique_ptr<std::istream> input_stream(new std::istream(&mem_instream));

    std::shared_ptr<IPort<BaseDataType> > source(new IstreamSource<BaseDataType>(
            "istream-1",
            static_cast<int>(total_num_data_bytes),
            std::move(input_stream)));
    std::shared_ptr<IPort<BaseDataType> > tomatrix(new ToMatrixConverter<BaseDataType>("tomatrix", dims));
    std::shared_ptr<IPort<BaseDataType> > block(new Transpose<BaseDataType>("transpose-1"));
    std::shared_ptr<IPort<BaseDataType> > sink(new ArchiverSink<BaseDataType>("archiver-1"));
    ArchiverSink<BaseDataType>* archive = dynamic_cast<ArchiverSink<BaseDataType>*>(sink.get());

    connect(source, connect(tomatrix, connect(block, sink)));

    REQUIRE(source);
    REQUIRE(tomatrix);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    BaseDataType expected_result[total_num_data_bytes];
    for (int i = 0; i < dims[0]; ++i) {
        for (int j = 0; j < dims[1]; ++j) {
            // the C representation of matrix is same as that support by
            // signal blocks (see IPort.h for details).
            expected_result[j * dims[0] + i] = buffer[i * dims[1] + j];
        }
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == 1);
    REQUIRE(archive->GetMatrixInfo().front().mTimeTick == timeTick);
    REQUIRE(archive->GetMatrixInfo().front().mDims == dims);
    for (size_t i = 0; i < total_num_data_bytes; ++i) {
        REQUIRE(archive->GetMatrixInfo().front().mData.get()[i] == expected_result[i]);
    }

    // run test in a loop to validate repeated data flow
    for (int i = 0; i < LOOP_COUNT; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetMatrixInfo().size() == (LOOP_COUNT + 1));
    REQUIRE(archive->GetMatrixInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetMatrixInfo().back().mDims == dims);
    for (size_t i = 0; i < total_num_data_bytes; ++i) {
        REQUIRE(archive->GetMatrixInfo().back().mData.get()[i] == expected_result[i]);
    }
}