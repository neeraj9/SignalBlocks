/// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
/// All rights reserved.

#include "../../blocks/sources/CsvFileSource.h"
#include "../../blocks/sinks/StdoutSink.h"
#include "../../blocks/sources/SelectiveCsvFileSource.h"
#include "../../common/logging.h"

using namespace sigblocks;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        LOG_ERROR("Need at least one csv input file\n");
        return 1;
    }

    //std::vector<std::string> select_columns{""};
    //std::shared_ptr<IPort<std::string> > source(new SelectiveCsvFileSource<1>(argv[1], select_columns));
    std::shared_ptr<IPort<std::string> > source(new CsvFileSource(argv[1], true));
    std::shared_ptr<IPort<std::string> > sink(new StdoutSink<std::string>());
    connect(source, sink);

    TimeTick time_tick(1);  // always start with non-zero value
    auto pCsvFileSource = dynamic_cast<CsvFileSource*>(source.get());
    while (! pCsvFileSource->IsDone()) {
        source->ClockCycle(time_tick);
        time_tick += 1;
    }
    return 0;
}