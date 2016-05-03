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

#include "../../blocks/converters/OrdinalConverter.h"
#include "../../blocks/basicop/Duplicator.h"
#include "../../blocks/basicop/Splitter.h"
#include "../../blocks/sources/CsvFileSource.h"
#include "../../blocks/sinks/StdoutSink.h"
#include "../../blocks/sources/SelectiveCsvFileSource.h"
#include "../../common/logging.h"

#include <sstream>


using namespace signalblocks;


std::shared_ptr<IPort<std::string> > CreateSimpleHistogramCodeBlocks(const std::string& csvFilename) {
    std::shared_ptr<IPort<std::string> > source(new CsvFileSource("csv-source-1", csvFilename, true));
    std::shared_ptr<IPort<std::string> > duplicator(new Duplicator<2, std::string>("duplicator-1"));
    std::shared_ptr<IPort<std::string> > sink(new StdoutSink<std::string>("[RAW CSV] "));

    std::vector<int> selective_indices;
    constexpr int num_columns = 4;
    selective_indices.reserve(num_columns);
    const std::vector<std::string>& header = dynamic_cast<CsvFileSource*>(source.get())->GetHeader();
    for (size_t i = 0; i < header.size(); ++i) {
        if (header[i] == "Country Code" ||
                header[i] == "Indicator Name" ||
                header[i] == "Indicator Code" ||
                header[i] == "2015") {
            selective_indices.push_back(i);
        }
    }
    std::shared_ptr<IPort<std::string> > splitter(new Splitter<num_columns, std::string>("splitter", ""));

    // connect as follows:
    //
    // source -> duplicator -> sink
    //               |
    //               +-------> splitter -> duplicator -> sink
    //                             |          |
    //                             |          +--------> ordinalconverter
    //                             |
    //                             +-----> duplicator -> sink
    //                             |          |
    //                             |          +--------> ordinalconverter
    //                             |
    //                             +-----> duplicator -> sink
    //                             |          |
    //                             |          +--------> ordinalconverter
    //                             |
    //                             +-----> duplicator -> sink
    //                                        |
    //                                        +--------> ordinalconverter
    //

    connect(source, connect(connect(duplicator, sink),
                            splitter, 1, 0));
    for (int i = 0; i < num_columns; ++i) {
        std::ostringstream out;
        out << "[SINK " << i << "] ";
        std::shared_ptr<IPort<std::string> > tmpsink(new StdoutSink<std::string>(out.str()));
        std::shared_ptr<IPort<std::string> > tmpduplicator(new Duplicator<2, std::string>("duplicator-block"));
        std::shared_ptr<IPort<std::string> > tmpconverter(new OrdinalConverter("ordinal-converter-block"));
        out.str(std::string()); // cleanup and clear contents of out
        out << "[CONVERTER SINK " << i << "] ";
        std::shared_ptr<IPort<unsigned long> > tmpconvertersink(new StdoutSink<unsigned long>(out.str()));
        connect(splitter, connect(connect(tmpduplicator,
                                          tmpsink),
                                  tmpconverter,
                                  1, 0),
                i, 0);
        connect(dynamic_cast<OrdinalConverter*>(tmpconverter.get())->GetAsSinkType(), tmpconvertersink);
    }

    return source;
}


int main(int argc, char* argv[]) {

    if (argc < 2) {
        LOG_ERROR("Need at least one csv input file\n");
        return 1;
    }

    //std::vector<std::string> select_columns{""};
    //std::shared_ptr<IPort<std::string> > source(new SelectiveCsvFileSource<1>(argv[1], select_columns));

    std::shared_ptr<IPort<std::string> > source = CreateSimpleHistogramCodeBlocks(argv[1]);
    TimeTick time_tick(1);  // always start with non-zero value
    auto pCsvFileSource = dynamic_cast<CsvFileSource*>(source.get());
    while (! pCsvFileSource->IsDone()) {
        source->ClockCycle(time_tick);
        time_tick += 1;
    }

    return 0;
}