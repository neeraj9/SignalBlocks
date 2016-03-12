// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "../blocks/converters/BasicTypeConverter.h"
#include "../blocks/math/Difference.h"
#include "../blocks/math/Gain.h"
#include "../blocks/math/Product.h"
#include "../blocks/math/Sum.h"
#include "../blocks/sinks/FileSinkCreator.h"
#include "../blocks/sinks/PlotSink.h"
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
#include "../blocks/transceivers/SocketTransceiverCreator.h"
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
#include "../plugins/PythonPlugin.h"

using namespace sigblocks;
using namespace std;

void test1() {
    // sample instantation
    std::shared_ptr<IPort<float> > port1(new Port<2, 2, float>());
    std::shared_ptr<IPort<float> > port2(new Port<2, 2, float>());
    port1->SetSink(port2, 0);
    port2->SetSource(port1.get(), 1);
}

void test2() {
    // lets do some data transfer
    std::unique_ptr<int[]> x(new int[10]);
    cout << "x = " << x.get() << endl;
    std::unique_ptr<int[]> y(std::move(x));
    cout << "x = " << x.get() << ", y = " << y.get() << endl;
}

void test3() {
    //bitset is optimized for size
    bitset<128> bit_set;
    cout << "sizeof(bitset<128>()) = " << sizeof(bit_set) << endl;
}

void test4() {
    std::shared_ptr<IPort<int> > pSource(new RandomSource);
    std::shared_ptr<IPort<int> > pSink(new StdoutSink<int>());
    connect(pSource, pSink);
    TimeTick time_tick(1);  // always start with 1
    for (int i = 0; i < 10; ++i) {
        pSource->ClockCycle(time_tick);
        time_tick += 1;
    }
}


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

void test6() {
    int block_size = 1;
    std::unique_ptr<istream> pIns(new ifstream("input.rf", ifstream::in | ifstream::binary));
    std::shared_ptr<IPort<short> > pSource(new ComplexStreamSource<short int>(std::move(pIns), block_size));
    std::unique_ptr<ostream> pOuts(new ofstream("output.rf", ofstream::out | ofstream::binary));
    std::shared_ptr<IPort<short> > pSink(new OstreamSink<short>(std::move(pOuts)));

    connect(pSource, pSink);

    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10; ++i) {
        pSource->ClockCycle(time_tick);
        time_tick += 1;
    }
    //pIns->close();
    //pOuts->close();
}

void test7() {
    int block_size = 1;
    std::shared_ptr<IPort<short> > pSource(
            FileSourceCreator<ComplexStreamSource, short>::Create(
                    "input.rf", block_size));
    std::shared_ptr<IPort<short> > pSink(
            FileSinkCreator<OstreamSink, short>::Create("output.rf"));
    connect(pSource, pSink);
    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10; ++i) {
        pSource->ClockCycle(time_tick);
        time_tick += 1;
    }
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

template<class T>
void SocketTransceiverTest(const string& inputFilename,
                           const string& outputFilename) {
    int block_size = 1;
    std::shared_ptr<IPort<unsigned char> > pSource(
            FileSourceCreator<IstreamSource, unsigned char>::Create(
                    inputFilename, block_size));

    std::shared_ptr<IPort<unsigned char> > pTransceiver(
            SocketSourceCreator<T>::Create(
                    block_size, "127.0.0.1", 12002, 12020));

    std::shared_ptr<IPort<unsigned char> > pSink(
            FileSinkCreator<OstreamSink, unsigned char>::Create(outputFilename));

    connect(pSource, connect(pTransceiver, pSink));

    // Note: We need to store only the pSource because
    // pTransceiver is captured inside pSource.
    // It is important to call Generate() on pTransceiver though,
    // so probably save the SharedPtr of pTransceiver anyways, but
    // remember to dispose that off once the pSource is gone. If this
    // is not done then its an issue later because pTransceiver will
    // point to a source which is nonexistent.

    cout << "waiting for data" << endl;
    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10; ++i) {
        // First trigger data from the file source and send outside the socket
        pSource->ClockCycle(time_tick);

        // Next get data from the socket and write to a file
        cout << "[" << i << "] waiting for data" << endl;
        sleep(2);
        pTransceiver->ClockCycle(time_tick);

        time_tick += 1;
    }
}

void test8() {
    SocketSourceTest<SocketProgramming::UdpSocket>("udp_port_12000.dat");
}

void test9() {
    SocketSinkTest<SocketProgramming::UdpSocket>();
}

void test10() {
    SocketTransceiverTest<SocketProgramming::UdpSocket>(
            "input.txt",
            "udp_port_12000.dat");
}

// TODO: Not tested
void test11() {
    SocketSourceTest<SocketProgramming::UdpLiteSocket>("udplite_port_12000.dat");
}

// TODO: Not tested
void test12() {
    SocketSinkTest<SocketProgramming::UdpLiteSocket>();
}

// TODO: Not tested
void test13() {
    SocketTransceiverTest<SocketProgramming::UdpLiteSocket>(
            "input.txt",
            "udp_port_12000.dat");
}

void test17() {
    std::shared_ptr<IPort<int> > pSource(new RandomSource);
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

void test18() {
    // XXX free to be used
}

void test21() {
    stack<int> s;

    for (int i = 0; i < 10; ++i) {
        s.push(i);
    }
    cout << "Reversed = ";
    for (int i = 0; i < 10; ++i) {
        cout << s.top() << ",";
        s.pop();
    }
    cout << endl;
}

void test22() {
    std::shared_ptr<IPort<int> > pSource(new ConstantSource<int>(10));
    std::shared_ptr<IPort<int> > pSink(new StdoutSink<int>());

    connect(pSource, pSink);

    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10; ++i) {
        pSource->ClockCycle(time_tick);
        time_tick += 1;
    }
}

void test23() {
    std::shared_ptr<IPort<int> > pSource1(new ConstantSource<int>(10));
    std::shared_ptr<IPort<int> > pSource2(new ConstantSource<int>(20));
    std::shared_ptr<IPort<int> > pSum(new Sum<2, int>());
    std::shared_ptr<IPort<int> > pSink(new StdoutSink<int>());

    connect(pSource1, pSum);
    connect(pSource2, pSum, 0, 1);
    connect(pSum, pSink);

    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10; ++i) {
        pSource1->ClockCycle(time_tick);
        pSource2->ClockCycle(time_tick);
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

void test25() {
    std::shared_ptr<IPort<int> > pSource(new ConstantSource<int>(10));
    std::shared_ptr<IPort<int> > pD(new Gain<int>(10));
    std::shared_ptr<IPort<int> > pSink(new StdoutSink<int>());

    connect(pSource, pD);
    connect(pD, pSink);

    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10; ++i) {
        pSource->ClockCycle(time_tick);
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

void test27() {
    std::shared_ptr<IPort<int> > pSource(new StepSource<int>(0));
    std::shared_ptr<IPort<int> > pSink(new PlotSink<int>());
    connect(pSource, pSink);

    TimeTick time_tick(1);  // always start with non-zero value
    for (int i = 0; i < 10000; ++i) {
        if (i % 500 == 0) {
            sleep(2);
        }
        pSource->ClockCycle(time_tick);
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

// TODO this is a raw test and instead use PythonNOperator
void test34() {
    PythonPlugin* pPyplugin = new PythonPlugin;
    try {
        std::string source("print 'Hello World!'");
        std::unique_ptr<PythonRunnableCode> runnable(
                pPyplugin->ParsePythonSource(source));
        if (runnable.get() == 0) {
            return; // XXX error!
        }

        std::cout << "Running inline source [" << source << "] : output {";
        std::unique_ptr<PythonBaseResult> result(
                pPyplugin->RunPythonRunnableCode(runnable.get()));
        std::cout << "} ";
        if (result.get() != 0) {
            std::cout << result->ToString() << std::endl;
        }
        else {
            std::cout << "No result returned!" << std::endl;
        }
    }
    catch (PyPluginTypeException& e) {
        std::cerr << e.what() << std::endl;
    }


    // test numpy code
    try {
        std::cout << "Running numpy_test.myrange : output {";
        // TODO the path needs to be where the numpy_test.py exists
        // TODO this is hackish and need to instead use other mechanism
        //      to get the correct source path to search for numpy_test.py
        std::unique_ptr<PythonBaseResult> result(
                pPyplugin->RunPythonCode("./", "numpy_test", "myrange"));
        std::cout << "} ";
        if (result.get() != 0) {
            std::cout << result->ToString() << std::endl;
        }
        else {
            std::cout << "No result returned!" << std::endl;
        }
    } catch (PyPluginTypeException& e) {
        std::cerr << e.what() << std::endl;
    }
}

int main() {
    test34();
    //test32();
    //test27();
    return 0;
}
