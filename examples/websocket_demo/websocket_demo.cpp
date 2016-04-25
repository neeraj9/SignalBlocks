// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// All rights reserved.

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

using namespace sigblocks;

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
    std::shared_ptr<IPort<BaseDataType> > source(new AudioSource<BaseDataType>(block_size, std::move(audioStream)));
    std::shared_ptr<IPort<BaseDataType> > block(new Splitter<2, BaseDataType>(0));
    std::shared_ptr<IPort<BaseDataType> > nullport(new Terminator<BaseDataType>);
    std::shared_ptr<IPort<BaseDataType> > sink(new JsonDataExtractableSink<BaseDataType>);
    JsonDataExtractableSink<BaseDataType>* archive = dynamic_cast<JsonDataExtractableSink<BaseDataType>*>(sink.get());

    JsonDataCallbackFuncType cb = std::bind(&JsonDataExtractableSink<BaseDataType>::GetAsJsonData,
                                            archive,
                                            std::placeholders::_1,
                                            std::placeholders::_2);
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
    std::shared_ptr<IPort<BaseDataType> > source(
            std::move(CreateWebsocketDemoCodeBlocks(
                    httpTcpWebsocketServer.get(),
                    std::move(audiofile))));

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