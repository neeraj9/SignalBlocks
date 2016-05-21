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
#include "../blocks/converters/BasicTypeConverter.h"
#include "../blocks/sources/IstreamSource.h"
#include "../blocks/text/SplitLineBlock.h"
#include "../blocks/text/TextCsvParser.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

using namespace signalblocks;

/// This test case is specific to the strings,
/// so beware while changing any part of it. The idea is to
/// test completely rather than generality, hence the approach.
/// Change this to generic form if it gets to messy.
TEST_CASE("Testing csv parser block for vector", "[csv parser block]") {
    // Important: must terminate with \n because otherwise
    // IstreamSource() will loop over and the last string will be read as
    // Hi Adam.What is your name?
    // So, terminating with '\n' solves the issue
    std::string data("What is,your name?\nMy name,is Adam.\nHi,Adam.\n");
    size_t total_num_data_bytes = data.size();
    int block_size = 4; // read 4 at a time
    constexpr bool has_header = false;

    MemInputStream mem_instream(const_cast<char*>(data.c_str()), total_num_data_bytes);
    std::unique_ptr<std::istream> input_stream(new std::istream(&mem_instream));
    std::shared_ptr<IPort<char> > source(new IstreamSource<char>("stream", block_size, std::move(input_stream)));
    std::shared_ptr<IPort<char> > linesplitter(new SplitLineBlock("line-splitter"));
    std::shared_ptr<IPort<char> > tostring(new BasicTypeConverter<char, std::string>("to-std-string"));
    std::shared_ptr<IPort<std::string> > block(new TextCsvParser("csv-stream-parser", has_header));
    std::shared_ptr<IPort<std::string> > sink(new ArchiverSink<std::string>("archiver-1"));
    ArchiverSink<std::string>* archive = dynamic_cast<ArchiverSink<std::string>*>(sink.get());
    BasicTypeConverter<char, std::string>* converter =
            dynamic_cast<BasicTypeConverter<char, std::string>*>(tostring.get());

    connect(source, connect(linesplitter, tostring));
    connect(converter->GetAsSinkType(), connect(block, sink));

    REQUIRE(source);
    REQUIRE(linesplitter);
    REQUIRE(tostring);
    REQUIRE(block);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(0);
    for (int i = 0; i < 4; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 0);

    timeTick += 1;
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 1);
    REQUIRE(archive->GetVectorInfo().back().mLen == 2);
    REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);
    {
        // validate the expectation
        std::string expected[2] = {"What is", "your name?"};
        REQUIRE(archive->GetVectorInfo().back().mData.get()[0] == expected[0]);
        REQUIRE(archive->GetVectorInfo().back().mData.get()[1] == expected[1]);
    }

    for (int i = 0; i < (35 - 19 + (block_size - 1))/block_size; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 2);
    REQUIRE(archive->GetVectorInfo().back().mLen == 2);
    REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);
    {
        // validate the expectation
        std::string expected[2] = {"My name" , "is Adam."};
        REQUIRE(archive->GetVectorInfo().back().mData.get()[0] == expected[0]);
        REQUIRE(archive->GetVectorInfo().back().mData.get()[1] == expected[1]);
    }

    for (int i = 0; i < 3; ++i) {
        timeTick += 1;
        source->ClockCycle(timeTick);
    }

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 3);
    REQUIRE(archive->GetVectorInfo().back().mLen == 2);
    REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);
    {
        // validate the expectation
        std::string expected[2] = {"Hi", "Adam."};
        REQUIRE(archive->GetVectorInfo().back().mData.get()[0] == expected[0]);
        REQUIRE(archive->GetVectorInfo().back().mData.get()[1] == expected[1]);
    }
}