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

#include "../../common/SB.h"
#include "../../common/PortType.h"
#include "../../blocks/basicop/Splitter.h"
#include "../../blocks/sources/AudioSource.h"
#include "../../blocks/sources/LinearSource.h"
#include "../../blocks/sinks/JsonDataExtractableSink.h"
#include "../../blocks/sinks/Terminator.h"
#include "../../blocks/math/Sin.h"
#include "../../blocks/math/Gain.h"
#include "../../blocks/math/Tan.h"
#include "../../common/logging.h"

#include <atomic>
#include <fstream>
#include <thread>
#include <unistd.h>
#include <HttpTcpWebsocketServer.h>

using namespace signalblocks;

using BaseDataType = short;

void RunHttpServer(HttpTcpWebsocketServer* pHttpTcpWebsocketServer,
                   std::atomic<bool>* pKeepRunning) {
    LOG_INFO("Running http tcp websocket server\n");
    while (std::atomic_load<bool>(pKeepRunning)) {
        pHttpTcpWebsocketServer->Run();
    }
    LOG_INFO("http server thread is terminating.\n");
}

std::shared_ptr<IPort<BaseDataType> > CreateWebsocketDemoCodeBlocks(
        HttpTcpWebsocketServer* pHttpTcpWebsocketServer,
        std::unique_ptr<std::istream> audioStream) {
    int block_size = 1; // one sample at a time
    auto source = CreateBlock<AudioSource<BaseDataType>,
            std::string, int&, std::unique_ptr<std::istream> >(std::string("audio-src-1"), block_size, std::move(audioStream));
    auto block = CreateBlock<Splitter<2, BaseDataType>, std::string, int>(std::string("splitter-1"), 0);
    auto nullport = CreateBlock<Terminator<BaseDataType>, std::string>(std::string("nullport-1"));
    auto sink = CreateBlock<JsonDataExtractableSink<BaseDataType>, std::string>(std::string("json-data-extractor-1"));
    JsonDataExtractableSink<BaseDataType>* archive = dynamic_cast<JsonDataExtractableSink<BaseDataType>*>(sink.get());

    JsonDataCallbackFuncType cb = archive->GetDataCallback();
    pHttpTcpWebsocketServer->AddRoute("/1", std::move(cb));

    connect(source, connect(connect(block, sink), nullport, 1, 0));

    return source;
}

int main(int argc, char* argv[]) {

    int http_port = 8585;

    if (argc != 2) {
        LOG_ERROR("Need at most one argument as a wav file.\n");
        return 1;
    }

    std::string filename(argv[1]);

    std::unique_ptr<HttpTcpWebsocketServer> httpTcpWebsocketServer(new HttpTcpWebsocketServer(http_port));
    std::unique_ptr<std::istream> audiofile(new std::ifstream(filename));
    std::shared_ptr<IPort<BaseDataType> > source = CreateWebsocketDemoCodeBlocks(
                    httpTcpWebsocketServer.get(),
                    std::move(audiofile));

    std::atomic<bool> keep_running(true);
    std::thread http_server(&RunHttpServer, httpTcpWebsocketServer.get(), &keep_running);

    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 1000000000; ++i) {
        source->ClockCycle(time_tick);
        time_tick += 1;
        usleep(200 * 1000);  // sleep for 200 milliseconds
    }

    LOG_INFO("inform http thread to terminate gracefully\n");
    std::atomic_store<bool>(&keep_running, false);

    http_server.join();

    return 0;
}
