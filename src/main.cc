// (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
// see LICENSE for license
#include "../blocks/converters/BasicTypeConverter.hh"
#include "../blocks/math/Difference.hh"
#include "../blocks/math/Gain.hh"
#include "../blocks/math/Product.hh"
#include "../blocks/math/Sum.hh"
#include "../blocks/sinks/FileSinkCreator.hh"
#include "../blocks/sinks/PlotSink.hh"
#include "../blocks/sinks/SocketSink.hh"
#include "../blocks/sinks/SocketSinkCreator.hh"
#include "../blocks/sinks/StdoutSink.hh"
#include "../blocks/sinks/OstreamSink.hh"
#include "../blocks/sources/ComplexStreamSource.hh"
#include "../blocks/sources/ConstantSource.hh"
#include "../blocks/sources/FileSourceCreator.hh"
#include "../blocks/sources/RandomSource.hh"
#include "../blocks/sources/IstreamSource.hh"
#include "../blocks/sources/SocketSource.hh"
#include "../blocks/sources/SocketSourceCreator.hh"
#include "../blocks/sources/StepSource.hh"
#include "../blocks/transceivers/SocketTransceiverCreator.hh"
#include "../common/ComplexType.hh"
#include "../common/IPort.hh"
#include "../common/MultiPtr.hh"
#include "../common/Port.hh"
#include "../common/TimeTick.hh"
#include "../socket/TcpSocket.hh"
#include "../socket/UdpLiteSocket.hh"
#include "../socket/UdpSocket.hh"

#include <fstream>
#include <iostream>
#include <bitset>
#include <stack>
#include <unistd.h> // for sleep
#include "../plugins/PythonPlugin.hh"

using namespace sigblocks;
using namespace std;

void test1()
{
  // sample instantation
  std::shared_ptr<IPort<float> > port1(new Port<2,2,float>());
  std::shared_ptr<IPort<float> > port2(new Port<2,2,float>());
  dynamic_cast<Port<2,2,float>*>(port1.get())->SetSink(port2, 0);
  dynamic_cast<Port<2,2,float>*>(port2.get())->SetSource(port1.get(), 1);
}

void test2()
{
  // lets do some data transfer
  MultiPtr<int> x(new int[10]);
  cout << "x = " << x.get() << endl;
  MultiPtr<int> y(x);
  cout << "x = " << x.get() << ", y = " << y.get() << endl;
}

void test3()
{
  //bitset is optimized for size
  bitset<128> bit_set;
  cout << "sizeof(bitset<128>()) = " << sizeof(bit_set) << endl;
}

void test4()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  RandomSource* pRandom_source = new RandomSource(start_time, increment);
  StdoutSink<int>* pStdout_sink = new StdoutSink<int>();
  std::shared_ptr<IPort<int> > pSource(pRandom_source);
  std::shared_ptr<IPort<int> > pSink(pStdout_sink);
  pRandom_source->SetSink(pSink, 0);
  pStdout_sink->SetSource(pSource.get(), 0);
  for (int i = 0; i < 10; ++i)
  {
    pRandom_source->Generate();
  }
}


void test5()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  int block_size = 1;
  IstreamSource<unsigned char>* pStream_source =
    new IstreamSource<unsigned char>(start_time, increment, block_size);
  OstreamSink<unsigned char>* pStream_sink = new OstreamSink<unsigned char>();
  std::shared_ptr<IPort<unsigned char> > pSource(pStream_source);
  std::shared_ptr<IPort<unsigned char> > pSink(pStream_sink);
  std::unique_ptr<istream> pIns(new ifstream("input.txt", ifstream::in | ifstream::binary));
  std::unique_ptr<ostream> pOuts(new ofstream("output.txt", ofstream::out | ofstream::binary));
  pStream_source->SetStreamSource(std::move(pIns));
  pStream_sink->SetStreamSink(std::move(pOuts));
  pStream_source->SetSink(pSink, 0);
  pStream_sink->SetSource(pSource.get(), 0);
  for (int i = 0; i < 10; ++i)
  {
    pStream_source->Generate();
  }
  //pIns->close();
  //pOuts->close();
}

void test6()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  int block_size = 1;
  ComplexStreamSource<short>* pStream_source = new ComplexStreamSource<short int>(start_time, increment, block_size);
  OstreamSink<short>* pStream_sink = new OstreamSink<short>();
  std::shared_ptr<IPort<short> > pSource(pStream_source);
  std::shared_ptr<IPort<short> > pSink(pStream_sink);
  std::unique_ptr<istream> pIns(new ifstream("input.rf", ifstream::in | ifstream::binary));
  std::unique_ptr<ostream> pOuts(new ofstream("output.rf", ofstream::out | ofstream::binary));
  pStream_source->SetStreamSource(std::move(pIns));
  pStream_sink->SetStreamSink(std::move(pOuts));
  pStream_source->SetSink(pSink, 0);
  pStream_sink->SetSource(pSource.get(), 0);
  for (int i = 0; i < 10; ++i)
  {
    pStream_source->Generate();
  }
  //pIns->close();
  //pOuts->close();
}

void test7()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  int block_size = 1;
  std::shared_ptr<IPort<short> > pSource(
    FileSourceCreator<ComplexStreamSource, short>::Create(
      start_time, increment, "input.rf", block_size));
  std::shared_ptr<IPort<short> > pSink(
    FileSinkCreator<OstreamSink, short>::Create("output.rf"));
  pSource->SetSink(pSink, 0);
  pSink->SetSource(pSource.get(), 0);
  for (int i = 0; i < 10; ++i)
  {
    dynamic_cast<ComplexStreamSource<short>*>(pSource.get())->Generate();
  }
}

template <class T>
void SocketSourceTest(const string& outputFilename)
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  int block_size = 1;
  std::shared_ptr<IPort<unsigned char> > pSource(
    SocketSourceCreator<T>::Create(
      start_time, increment, block_size, "127.0.0.1", 0, 12000));
  std::shared_ptr<IPort<unsigned char> > pSink(
    FileSinkCreator<OstreamSink, unsigned char>::Create(outputFilename));
  pSource->SetSink(pSink, 0);
  pSink->SetSource(pSource.get(), 0);
  cout << "waiting for data" << endl;
  for (int i = 0; i < 10; ++i)
  {
    cout << "[" << i << "] waiting for data" << endl;
    sleep(2);
    dynamic_cast<SocketSource*>(pSource.get())->Generate();
  }
}

template <class T>
void SocketSinkTest()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  int block_size = 1;
  std::shared_ptr<IPort<unsigned char> > pSource(
    FileSourceCreator<IstreamSource, unsigned char>::Create(
      start_time, increment, "input.txt", block_size));
  // localPort is optional for udp
  std::shared_ptr<IPort<unsigned char> > pSink(
    SocketSinkCreator<T>::Create("127.0.0.1", 12002, 12020));
  pSource->SetSink(pSink, 0);
  pSink->SetSource(pSource.get(), 0);

  // Note: We just need to store the SharedPtr of pSource and not
  // for any block in-between or the sink, because they will
  // be captured in the tx.

  for (int i = 0; i < 10; ++i)
  {
    dynamic_cast<IstreamSource<unsigned char>*>(pSource.get())->Generate();
  }
}

template <class T>
void SocketTransceiverTest(const string& inputFilename,
                           const string& outputFilename)
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  int block_size = 1;
  std::shared_ptr<IPort<unsigned char> > pSource(
    FileSourceCreator<IstreamSource, unsigned char>::Create(
      start_time, increment, inputFilename, block_size));

  std::shared_ptr<IPort<unsigned char> > pTransceiver(
    SocketSourceCreator<T>::Create(
      start_time, increment, block_size, "127.0.0.1", 12002, 12020));

  std::shared_ptr<IPort<unsigned char> > pSink(
    FileSinkCreator<OstreamSink, unsigned char>::Create(outputFilename));

  pSource->SetSink(pTransceiver, 0);
  pTransceiver->SetSource(pSource.get(), 0);
  pTransceiver->SetSink(pSink, 0);
  pSink->SetSource(pTransceiver.get(), 0);

  // Note: We need to store only the pSource because
  // pTransceiver is captured inside pSource.
  // It is important to call Generate() on pTransceiver though,
  // so probably save the SharedPtr of pTransceiver anyways, but
  // remember to dispose that off once the pSource is gone. If this
  // is not done then its an issue later because pTransceiver will
  // point to a source which is nonexistent.

  cout << "waiting for data" << endl;
  for (int i = 0; i < 10; ++i)
  {
    // First trigger data from the file source and send outside the socket
    dynamic_cast<IstreamSource<unsigned char>*>(pSource.get())->Generate();

    // Next get data from the socket and write to a file
    cout << "[" << i << "] waiting for data" << endl;
    sleep(2);
    dynamic_cast<SocketTransceiver*>(pTransceiver.get())->Generate();
  }
}

void test8()
{
  SocketSourceTest<SocketProgramming::UdpSocket>("udp_port_12000.dat");
}

void test9()
{
  SocketSinkTest<SocketProgramming::UdpSocket>();
}

void test10()
{
  SocketTransceiverTest<SocketProgramming::UdpSocket>(
    "input.txt",
    "udp_port_12000.dat");
}

// TODO: Not tested
void test11()
{
  SocketSourceTest<SocketProgramming::UdpLiteSocket>("udplite_port_12000.dat");
}

// TODO: Not tested
void test12()
{
  SocketSinkTest<SocketProgramming::UdpLiteSocket>();
}

// TODO: Not tested
void test13()
{
  SocketTransceiverTest<SocketProgramming::UdpLiteSocket>(
    "input.txt",
    "udp_port_12000.dat");
}

void test17()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);

  RandomSource* pRandom_source = new RandomSource(start_time, increment);
  BasicTypeConverter<int,float>* converter = new BasicTypeConverter<int,float>();
  StdoutSink<float>* pStdout_sink = new StdoutSink<float>();

  std::shared_ptr<IPort<int> > pSource(pRandom_source);
  std::shared_ptr<IPort<int> > pConverter_sink(converter);
  std::shared_ptr<IPort<float> > pSink(pStdout_sink);

  pRandom_source->SetSink(pConverter_sink, 0);
  converter->SetSource(pSource.get(), 0);
  converter->SetSink(pSink, 0);
  pStdout_sink->SetSource(converter, 0);
  for (int i = 0; i < 10; ++i)
  {
    pRandom_source->Generate();
  }
}

void test18()
{
  // XXX free to be used
}

void test21()
{
  stack<int> s;

  for (int i = 0; i < 10; ++i)
  {
    s.push(i);
  }
  cout << "Reversed = ";
  for (int i = 0; i < 10; ++i)
  {
    cout << s.top() << ",";
    s.pop();
  }
  cout << endl;
}

void test22()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  ConstantSource<int>* pConstant_source =
    new ConstantSource<int>(start_time, increment, 10);
  StdoutSink<int>* pStdout_sink = new StdoutSink<int>();
  std::shared_ptr<IPort<int> > pSource(pConstant_source);
  std::shared_ptr<IPort<int> > pSink(pStdout_sink);
  pConstant_source->SetSink(pSink, 0);
  pStdout_sink->SetSource(pSource.get(), 0);
  for (int i = 0; i < 10; ++i)
  {
    pConstant_source->Generate();
  }
}

void test23()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  ConstantSource<int>* pC1 =
    new ConstantSource<int>(start_time, increment, 10);
  ConstantSource<int>* pC2 =
    new ConstantSource<int>(start_time, increment, 20);
  Sum<2, int>* pS1 = new Sum<2, int>();
  StdoutSink<int>* pStdout_sink = new StdoutSink<int>();

  std::shared_ptr<IPort<int> > pSource1(pC1);
  std::shared_ptr<IPort<int> > pSource2(pC2);
  std::shared_ptr<IPort<int> > pSum(pS1);
  std::shared_ptr<IPort<int> > pSink(pStdout_sink);
  pSource1->SetSink(pSum, 0);
  pSource2->SetSink(pSum, 0);
  pSum->SetSource(pSource1.get(), 0);
  pSum->SetSource(pSource2.get(), 1);
  pSum->SetSink(pSink, 0);
  pStdout_sink->SetSource(pSum.get(), 0);
  for (int i = 0; i < 10; ++i)
  {
    pC1->Generate();
    pC2->Generate();
  }
}

void test24()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  ConstantSource<int>* pC1 =
    new ConstantSource<int>(start_time, increment, 20);
  ConstantSource<int>* pC2 =
    new ConstantSource<int>(start_time, increment, 10);
  Difference<2, int>* pS1 = new Difference<2, int>();
  StdoutSink<int>* pStdout_sink = new StdoutSink<int>();

  std::shared_ptr<IPort<int> > pSource1(pC1);
  std::shared_ptr<IPort<int> > pSource2(pC2);
  std::shared_ptr<IPort<int> > pD(pS1);
  std::shared_ptr<IPort<int> > pSink(pStdout_sink);
  pSource1->SetSink(pD, 0);
  pSource2->SetSink(pD, 0);
  pD->SetSource(pSource1.get(), 0);
  pD->SetSource(pSource2.get(), 1);
  pD->SetSink(pSink, 0);
  pStdout_sink->SetSource(pD.get(), 0);
  for (int i = 0; i < 10; ++i)
  {
    pC1->Generate();
    pC2->Generate();
  }
}

void test25()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  ConstantSource<int>* pC1 =
    new ConstantSource<int>(start_time, increment, 10);
  Gain<int>* pS1 = new Gain<int>(10);
  StdoutSink<int>* pStdout_sink = new StdoutSink<int>();

  std::shared_ptr<IPort<int> > pSource1(pC1);
  std::shared_ptr<IPort<int> > pD(pS1);
  std::shared_ptr<IPort<int> > pSink(pStdout_sink);
  pSource1->SetSink(pD, 0);
  pD->SetSource(pSource1.get(), 0);
  pD->SetSink(pSink, 0);
  pStdout_sink->SetSource(pD.get(), 0);
  for (int i = 0; i < 10; ++i)
  {
    pC1->Generate();
  }
}

void test26()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  ConstantSource<int>* pC1 =
    new ConstantSource<int>(start_time, increment, 10);
  ConstantSource<int>* pC2 =
    new ConstantSource<int>(start_time, increment, 20);
  Product<2, int>* pS1 = new Product<2, int>();
  StdoutSink<int>* pStdout_sink = new StdoutSink<int>();

  std::shared_ptr<IPort<int> > pSource1(pC1);
  std::shared_ptr<IPort<int> > pSource2(pC2);
  std::shared_ptr<IPort<int> > pP(pS1);
  std::shared_ptr<IPort<int> > pSink(pStdout_sink);
  pSource1->SetSink(pP, 0);
  pSource2->SetSink(pP, 0);
  pP->SetSource(pSource1.get(), 0);
  pP->SetSource(pSource2.get(), 1);
  pP->SetSink(pSink, 0);
  pStdout_sink->SetSource(pP.get(), 0);
  for (int i = 0; i < 10; ++i)
  {
    pC1->Generate();
    pC2->Generate();
  }
}

void test27()
{
  TimeTick start_time(0.0f);
  TimeTick increment(1.0f);
  StepSource<int>* pStep_source =
    new StepSource<int>(start_time, increment, 0);
  PlotSink<int>* pPlot_sink = new PlotSink<int>();
  std::shared_ptr<IPort<int> > pSource(pStep_source);
  std::shared_ptr<IPort<int> > pSink(pPlot_sink);
  pStep_source->SetSink(pSink, 0);
  pPlot_sink->SetSource(pSource.get(), 0);
  for (int i = 0; i < 10000; ++i)
  {
    if (i % 500 == 0)
    {
      sleep(2);
    }
    pStep_source->Generate();
  }
}

void test30()
{
  // XXX test TcpSocket source
  SocketSourceTest<SocketProgramming::TcpSocket>("tcp_port_12000.dat");
}

void test31()
{
  // XXX test TcpSocket sink
  SocketSinkTest<SocketProgramming::TcpSocket>();
}

void test32()
{
  // wow! this works like c structure.
  // TODO: Will this work all the time, for all optimization flag?
  printf("sizeof(double) = %d\n", sizeof(double));
  printf("sizeof(ComplexType<double> = %d\n", sizeof(ComplexType<double>));

  double v[2] = {1.0, 2.0};
  ComplexType<double>* p = reinterpret_cast<ComplexType<double>*>(&v[0]);
  printf("real = %g, imag = %g\n", p->GetReal(), p->GetImag());

  // another way to reinterpret native (double) to complex type.
  p = ToComplex<double>(&v[0]);
  printf("real = %g, imag = %g\n", p->GetReal(), p->GetImag());
}

// TODO this is a raw test and instead use PythonNOperator
void test34()
{
    PythonPlugin* pPyplugin = new PythonPlugin;
  try
  {
    std::string source("print 'Hello World!'");
    std::unique_ptr<PythonRunnableCode> runnable(
            pPyplugin->ParsePythonSource(source));
    if (runnable.get() == 0)
    {
      return; // XXX error!
    }

    std::cout << "Running inline source [" << source << "] : output {";
    std::unique_ptr<PythonBaseResult> result(
            pPyplugin->RunPythonRunnableCode(runnable.get()));
    std::cout << "} ";
    if (result.get() != 0)
    {
      std::cout << result->ToString() << std::endl;
    }
    else
    {
      std::cout << "No result returned!" << std::endl;
    }
  }
  catch (PyPluginTypeException& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

int main()
{
    test34();
  //test32();
  //test27();
  return 0;
}
