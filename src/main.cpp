// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "../blocks/basicop/Multiplex.h"
#include "../blocks/converters/BasicTypeConverter.h"
#include "../blocks/filters/DownSample.h"
#include "../blocks/math/Difference.h"
#include "../blocks/math/Gain.h"
#include "../blocks/math/Product.h"
#include "../blocks/math/Sum.h"
#include "../blocks/sinks/FileSinkCreator.h"
#include "../blocks/sinks/SocketSink.h"
#include "../blocks/sinks/SocketSinkCreator.h"
#include "../blocks/sinks/StdoutSink.h"
#include "../blocks/sinks/OstreamSink.h"
#include "../blocks/sources/ComplexStreamSource.h"
#include "../blocks/sources/ConstantSource.h"
#include "../blocks/sources/FileSourceCreator.h"
#include "../blocks/sources/RandomSource.h"
#include "../blocks/sources/IstreamSource.h"
#include "../blocks/sources/SocketSource.h"
#include "../blocks/sources/SocketSourceCreator.h"
#include "../blocks/sources/StepSource.h"
#include "../blocks/sources/CsvFileSource.h"
#include "../common/ComplexType.h"
#include "../common/IPort.h"
#include "../common/Port.h"
#include "../common/TimeTick.h"
#include "../socket/TcpSocket.h"
#include "../socket/UdpLiteSocket.h"
#include "../socket/UdpSocket.h"

#include <fstream>
#include <iostream>
#include <bitset>
#include <stack>


#include <unistd.h> // for sleep


using namespace sigblocks;
using namespace std;

void test5() {
    int block_size = 1;

    std::unique_ptr<istream> pIns(new ifstream("input.txt", ifstream::in | ifstream::binary));
    std::shared_ptr<IPort<unsigned char> > pSource(
            new IstreamSource<unsigned char>(block_size, std::move(pIns)));

    std::unique_ptr<ostream> pOuts(new ofstream("output.txt", ofstream::out | ofstream::binary));
    std::shared_ptr<IPort<unsigned char> > pSink(new OstreamSink<unsigned char>(std::move(pOuts)));

    connect(pSource, pSink);
    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10; ++i) {
        pSource->ClockCycle(time_tick);
        time_tick += 1;
    }
    //pIns->close();
    //pOuts->close();
}

template<class T>
void SocketSourceTest(const string& outputFilename) {
    int block_size = 1;
    std::shared_ptr<IPort<unsigned char> > pSource(
            SocketSourceCreator<T>::Create(
                    block_size, "127.0.0.1", 0, 12000));
    std::shared_ptr<IPort<unsigned char> > pSink(
            FileSinkCreator<OstreamSink, unsigned char>::Create(outputFilename));
    connect(pSource, pSink);
    cout << "waiting for data" << endl;
    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10; ++i) {
        cout << "[" << i << "] waiting for data" << endl;
        sleep(2);
        pSource->ClockCycle(time_tick);
        time_tick += 1;
    }
}

template<class T>
void SocketSinkTest() {
    int block_size = 1;
    std::shared_ptr<IPort<unsigned char> > pSource(
            FileSourceCreator<IstreamSource, unsigned char>::Create(
                    "input.txt", block_size));
    // localPort is optional for udp
    std::shared_ptr<IPort<unsigned char> > pSink(
            SocketSinkCreator<T>::Create("127.0.0.1", 12002, 12020));
    connect(pSource, pSink);

    // Note: We just need to store the SharedPtr of pSource and not
    // for any block in-between or the sink, because they will
    // be captured in the tx.
    TimeTick time_tick(1);  // always start with non-zero value

    for (int i = 0; i < 10; ++i) {
        pSource->ClockCycle(time_tick);
        time_tick += 1;
    }
}

void test8() {
    SocketSourceTest<SocketProgramming::UdpSocket>("udp_port_12000.dat");
}

void test9() {
    SocketSinkTest<SocketProgramming::UdpSocket>();
}

// TODO: Not tested
void test11() {
    SocketSourceTest<SocketProgramming::UdpLiteSocket>("udplite_port_12000.dat");
}

// TODO: Not tested
void test12() {
    SocketSinkTest<SocketProgramming::UdpLiteSocket>();
}

void test17() {
    std::shared_ptr<IPort<int> > pSource(new RandomSource<int>);
    BasicTypeConverter<int, float>* pBasicTypeConverter = new BasicTypeConverter<int, float>();
    std::shared_ptr<IPort<int> > pConverter_sink(pBasicTypeConverter);
    std::shared_ptr<IPort<float> > pSink(new StdoutSink<float>());

    connect(pSource, pConverter_sink);
    connect(pBasicTypeConverter->GetAsSinkType(), pSink);

    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10; ++i) {
        pSource->ClockCycle(time_tick);
        time_tick += 1;
    }
}

void test24() {
    std::shared_ptr<IPort<int> > pSource1(new ConstantSource<int>(20));
    std::shared_ptr<IPort<int> > pSource2(new ConstantSource<int>(10));
    std::shared_ptr<IPort<int> > pD(new Difference<2, int>());
    std::shared_ptr<IPort<int> > pSink(new StdoutSink<int>());

    connect(pSource1, pD);
    connect(pSource2, pD, 0, 1);
    connect(pD, pSink);

    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10; ++i) {
        pSource1->ClockCycle(time_tick);
        pSource2->ClockCycle(time_tick);
        time_tick += 1;
    }
}

void test26() {
    std::shared_ptr<IPort<int> > pSource1(new ConstantSource<int>(10));
    std::shared_ptr<IPort<int> > pSource2(new ConstantSource<int>(20));
    std::shared_ptr<IPort<int> > pP(new Product<2, int>());
    std::shared_ptr<IPort<int> > pSink(new StdoutSink<int>());

    connect(pSource1, pP);
    connect(pSource2, pP, 0, 1);
    connect(pP, pSink);

    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10; ++i) {
        pSource1->ClockCycle(time_tick);
        pSource2->ClockCycle(time_tick);
        time_tick += 1;
    }
}

void test30() {
    // XXX test TcpSocket source
    SocketSourceTest<SocketProgramming::TcpSocket>("tcp_port_12000.dat");
}

void test31() {
    // XXX test TcpSocket sink
    SocketSinkTest<SocketProgramming::TcpSocket>();
}

void test32() {
    // wow! this works like c structure.
    // TODO: Will this work all the time, for all optimization flag?
    printf("sizeof(double) = %lu\n", sizeof(double));
    printf("sizeof(ComplexType<double> = %lu\n", sizeof(ComplexType<double>));

    double v[2] = {1.0, 2.0};
    ComplexType<double>* p = reinterpret_cast<ComplexType<double>*>(&v[0]);
    printf("real = %g, imag = %g\n", p->GetReal(), p->GetImag());

    // another way to reinterpret native (double) to complex type.
    p = ToComplex<double>(&v[0]);
    printf("real = %g, imag = %g\n", p->GetReal(), p->GetImag());
}

void test35() {
    std::shared_ptr<IPort<std::string> > source(new CsvFileSource("test.csv", true));
    std::shared_ptr<IPort<std::string> > sink(new StdoutSink<std::string>());
    connect(source, sink);
}

void test36() {
    int default_value = 0;
    std::shared_ptr<IPort<int> > block(new Multiplex<2, int, MULTIPLEX_VECTOR>(default_value));
}

void test37() {
    int factor = 10;  // down sample to 1 in 10 TimeTick
    std::shared_ptr<IPort<int> > block(new DownSample<int>(factor));
}

int main() {
    //test32();
    //test27();
    return 0;
}
