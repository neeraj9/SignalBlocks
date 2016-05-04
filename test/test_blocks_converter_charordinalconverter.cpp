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
#include "../blocks/converters/CharOrdinalConverter.h"
#include "../blocks/sources/IstreamSource.h"
#include "../blocks/text/SplitWordsBlock.h"
#include "../blocks/sinks/ArchiverSink.h"

#include "catch.hpp"

using namespace signalblocks;

/// This test case is specific to the string "What is your name?"
/// so beware while changing any part of it. The idea is to
/// test completely rather than generality, hence the approach.
/// Change this to generic form if it gets to messy.
TEST_CASE("Testing char ordinal converter block for vector", "[char ordinal converter block]") {
    // 10 pairs of (I,Q)
    std::string data("What is your name?");
    size_t total_num_data_bytes = data.size();
    int block_size = 2; // read 2 at a time

    MemInputStream mem_instream(const_cast<char*>(data.c_str()), total_num_data_bytes);
    std::unique_ptr<std::istream> input_stream(new std::istream(&mem_instream));
    std::shared_ptr<IPort<char> > source(new IstreamSource<char>("stream", block_size, std::move(input_stream)));
    std::shared_ptr<IPort<char> > block(new SplitWordsBlock("word-splitter"));
    std::shared_ptr<IPort<char> > ordblock(new CharOrdinalConverter("char-ordinal-converter"));
    std::shared_ptr<IPort<unsigned long> > sink(new ArchiverSink<unsigned long>("archiver-1"));
    ArchiverSink<unsigned long>* archive = dynamic_cast<ArchiverSink<unsigned long>*>(sink.get());

    connect(source, connect(block, ordblock));
    connect(dynamic_cast<CharOrdinalConverter*>(ordblock.get())->GetAsSinkType(), sink);

    REQUIRE(source);
    REQUIRE(block);
    REQUIRE(ordblock);
    REQUIRE(sink);
    REQUIRE(archive);

    TimeTick timeTick(1);
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 0);
    //REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);

    timeTick += 1;
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().empty());
    REQUIRE(archive->GetVectorInfo().size() == 0);
    //REQUIRE(archive->GetVectorInfo().back().mTimeTick == timeTick);

    timeTick += 1;
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().size() == 1);
    REQUIRE(archive->GetScalarInfo().back().mData == 0);
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetVectorInfo().empty());

    timeTick += 1;
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().size() == 2);
    REQUIRE(archive->GetScalarInfo().back().mData == 1);
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetVectorInfo().empty());

    timeTick += 1;
    source->ClockCycle(timeTick);
    timeTick += 1;
    source->ClockCycle(timeTick);
    timeTick += 1;
    source->ClockCycle(timeTick);

    REQUIRE(archive->GetScalarInfo().size() == 3);
    REQUIRE(archive->GetScalarInfo().back().mData == 2);
    REQUIRE(archive->GetScalarInfo().back().mTimeTick == timeTick);
    REQUIRE(archive->GetVectorInfo().empty());
}