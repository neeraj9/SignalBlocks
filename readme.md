# SignalBlocks

[![Build Status](https://travis-ci.org/neeraj9/SignalBlocks.svg?branch=master)](https://travis-ci.org/neeraj9/SignalBlocks)
[![Coverage Status](https://coveralls.io/repos/github/neeraj9/SignalBlocks/badge.svg?branch=master)](https://coveralls.io/github/neeraj9/SignalBlocks?branch=master)
[![Software License](https://img.shields.io/badge/license-GPLv3-blue.svg?style=flat-square)](LICENSE)

The SignalBlocks is a c++11 framework to implement blocks of
logic which can be tied together. The concept inspired from
the Simulink modeling in Matlab but this framework can be used
for multiple applications.  This framework aims to be fast, adaptable
and free (GPLv3 license). Although I initially intended this for the
student and research community, but this tool is generic enough to
be used elsewhere.

> This framework tries to achieve standardisation in data interchange
> and module writing, which allows plug-and-play in a unique way.
> The approach is simplistically raw data interface with pre-defined
> contracts between the connected modules (or blocks) before
> they start working on the data. There is no special encoding or
> decoding of data although the framework is generic enough to
> allow anything complex.


This is a GPLv3 licensed software, so anything which links or compiles with it
must take note of the license. This may not look attractive while
developing proprietary or closed source implementations, but then you can
use it with outside your application by using the IPC (socket) blocks
to send and receive data. In that case your application just need to open
socket interface for receiving and sending data without being linked directly.
Since the interface is raw data, so there is no data encoding/decoding involved
other than data representation of various types in memory.

## Potential Applications

> The framework has the potential to be used in multiple areas although
> not limited to the following. Additionally, this is just a framework
> so it basically sets way but do not show the complete path.

* Data Analytics: Realtime, streaming data analytics.
* Digital Signal Processing: Various DSP related simulations.

## In-The-Works

The author is busy (apart from other enhancements) in finalizing a front-end
for creating a standalone application which dynamically creates network
and runs simulations.

> There are a number of javascript frameworks to get the job done,
> while contribution will be definitely of great help.

## Software Dependencies

This framework supports *Gnu/Linux* operating system, but with little change
it can be made to build on any of the POSIX compliant operating system. The
following depencies are required to use this project; namely

* [g++](https://gcc.gnu.org/) v3.8+ or [clang](http://clang.llvm.org/) v3.3+
* [cmake](https://cmake.org/)
* [GSL](http://www.gnu.org/software/gsl/)
* [libsndfile](http://www.mega-nerd.com/libsndfile/)
* [python-dev](http://www.python.org)
* [numpy](http://www.numpy.org/)

> The standard compatibility for
> [clang cxx status](http://clang.llvm.org/cxx_status.html) and
> [gcc cxx11 status](https://gcc.gnu.org/projects/cxx-status.html#cxx11)
> gives details of the appropriate compiler versions to use.

[Travis config](.travis.yml) sets up the ubuntu plaform for the build (hint:
look at the apt-get commands).

## Data Types

The following base data types are largely supported within the framework, but
this framework do not restrict you in any way. Other than being GPLv3 this
framework relies on templates heavily, so that new data type can be easily
added.

* char, unsigned char
* short, unsigned short
* int, unsigned int
* long, unsigned long
* float
* double, long double

The framework supports scalar, vector and N-Dimensional matrix and each of
the blocks must support any of them (unless intentionally not supported
where assertions are to be made).

## A Sneak Peak

"The proof is in the pudding ..." and the following demo should get sufficient
information (and possibly motivation) to decide on this project.

Lets take a sneak peak at a sample c++ source which reads
audio from a file and then stream the output to a remote
html5 app via a websocket server. The demo can be built via the
"websocket_demo" target in the cmake build rule. Cool huh!


```c++

    using BaseDataType = short;
    using BlockSharedPointer = std::shared_ptr<IPort<BaseDataType> >;

    void RunHttpServer(HttpTcpWebsocketServer* pHttpTcpWebsocketServer,
                       std::atomic<bool>* pKeepRunning) {
        LOG_INFO("Running http tcp websocket server\n");
        while (std::atomic_load<bool>(pKeepRunning)) {
            pHttpTcpWebsocketServer->Run();
        }
        LOG_INFO("http server thread is terminating.\n");
    }

    void run_signal_blocks(const char *filename) {
        // create the blocks and connect them together
        std::unique_ptr<std::istream> audiofile(new std::ifstream(filename));
        BlockSharedPointer source(new AudioSource<BaseDataType>(
            "audio-source", block_size, std::move(audioStream)));

        BlockSharedPointer block(new Splitter<2, BaseDataType>("extract-ch0", 0));
        BlockSharedPointer nullport(new Terminator<BaseDataType>("nullport"));
        BlockSharedPointer sink(
            new JsonDataExtractableSink<BaseDataType>("json-data-extractor"));

        connect(source, connect(connect(block, sink), nullport, 1, 0));
        // all the blocks are now connected and ready to run

        // create a websocket server thread
        std::atomic<bool> keep_running(true);
        std::unique_ptr<HttpTcpWebsocketServer> server(
            new HttpTcpWebsocketServer(http_port));

        // connect the json data block sink with websocket server
        JsonDataExtractableSink<BaseDataType>* archive =
            dynamic_cast<JsonDataExtractableSink<BaseDataType>*>(sink.get());
        JsonDataCallbackFuncType cb = archive->GetDataCallback();

        server->AddRoute("/1", std::move(cb));
        std::thread http_server(&RunHttpServer, server.get(), &keep_running);
        // now you can browse at http://<ip>:<port>/1 for websocket live
        // streaming data

        // run a demo simulation loop
        TimeTick time_tick(1);  // always start with non-zero value
        for (int i = 0; i < 1000000000; ++i) {
            source->ClockCycle(time_tick);
            time_tick += 1;
            usleep(200 * 1000);  // sleep for 200 milliseconds
        }
```

## Development Environment Setup

Although the installation for various GNU/Linux distributions differ but
the dependencies are easily available on any one of them.

### Ubuntu or Debian

The following commands were tested on Ubuntu 14.04 but things should be similar
(if not same) on other releases and Debian.

The following commands needs to be as a system administrator or with sudo
(as shown below) so that the relavent packages are installed in the
system.

If your system is not updated then probably it is a good idea to do that
before installing anything else.

    sudo apt-get update

In case you prefer the GNU GCC compler then install g++

    sudo apt-get install -y g++

Alternatively, you can install the clang compiler as well

    sudo apt-get install -y clang-3.5

The following other depencies are required as mentioned before.

    sudo apt-get install -y libsndfile1-dev libgsl0-dev python-numpy

### CentOS / Fedora / Redhat

The following commands were tested on Centos 7 but things should be similar
(if not same) on other releases and Fedora or Redhat.

If your system is not updated then probably it is a good idea to do that
before installing anything else.

    sudo yum update

In case you prefer the GNU GCC compler then install g++

    sudo yum install -y gcc-c++

Alternatively, you can install the clang compiler as well

    sudo yum install -y clang

The following other depencies are required as mentioned before.

    sudo yum install -y gsl-devel libsndfile-devel python-devel numpy

> The versions gcc (g++) and clang which are installed in your (rpm based)
> operating needs to meet the previously mentioned criteria. At the time of
> this writing CentOS 7, Fedora 23, RHEL 7 are the latest releases and works
> well. Anything earlier may not work for you unless you are willing to
> get under the hood and build things on your own.

### Archlinux

If your system is not updated then probably it is a good idea to do that
before installing anything else.

    sudo pacman -Syu

In case you prefer the GNU GCC compler then install g++

    sudo pacman -Sy gcc-multilib

Alternatively, you can install the clang compiler as well

    sudo pacman -Sy clang

The following other depencies are required as mentioned before.

    sudo yum install -y gsl libsndfile python2 python2-numpy

## Motivation

I was always fascinated by Matlab's power and ease of use while in
college and wanted to implement something similar, but in a language
which can be compiled to native format for fast performance.
This project is an attempt to give a basic framework which will (soon)
encapsulate many of the open source software to construct a building
block for such requirement.

## Build and Test

After you are done setting up the development environment the build is
pretty straight-forward (see below).

    git clone https://github.com/neeraj9/SignalBlocks
    cd SignalBlocks
    mkdir build
    cd build
    cmake ..
    make

The tests are integrated into the ctest framework, so you can run it
simply as follows:

    make test

The testsuite is another target which is built and run as follows (in case
you want to run that directly instead of via the ctest way):

    make testsuite
    ./test/testsuite

Take a look at the example projects for using this framework. Lets build the
websocket_demo (for example).

    cd SignalBlocks/build
    make websocket_demo
    ./examples/websocket_demo/websocket_demo somewavfile.wav


## Components

The framework has the following components:

* core - The core of the library which forms the basis for writing usable blocks.
* socket - The socket related wrapper code (over system socket api) which is
  used by some of the blocks.
* blocks - The base blocks which provide a lot of useful functionality (for example sources, sinks, etc).
* http - A primitive (yet working) websocket server which hosts data results for
  websocket clients to view. There is even a cool demo in the examples subfolder.
* examples - Example applications to use the framework.
* test - Unit test based on a great (single header) test framework
  [Catch](https://github.com/philsquared/Catch).

## Blocks

The blocks are categorized in as follows:

1. basicop - These blocks are used for basic operations like buffer, demux, etc.
2. converters - It converts input say to-ordinal, or to-matrix.
3. filters - It hosts the delay and down sample blocks at present, but primarily
             these should contain mathematical filter blocks.
4. math - All the blocks are basic mathematical operations like sum, product, cos, sin
          and others.
5. python - It is supposed to (untested at present) allow generic filters to be
            created from python code. The python plugin works (and is unit-tested
            as well but the code block is not).
6. sinks - The blocks provide various options for archiving, storing or looking
           at the signals.
7. sources - These are some of the basic signal (or data) sources like audio,
             file, constant, step, linear and others.
8. text - The blocks provide functionality to operate on text and
          seldom care about the time tick (so take note).

The following list of blocks are available within the system (as on date) although
not all of them are thoroughly tested.

### BasicOp - Basic Operations Blocks

<table>
    <thead>
        <tr>
            <th>Block</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><a href="blocks/basicop/Demultiplex.h">de-multiplexer</a></td>
            <td>one to many</td>
        </tr>
        <tr>
            <td><a href="blocks/basicop/Multiplex.h">multiplexer</a></td>
            <td>many to one</td>
        </tr>
        <tr>
            <td><a href="blocks/basicop/Buffer.h">buffer</a></td>
            <td>convert serial data to a buffer of given size</td>
        </tr>
        <tr>
            <td><a href="blocks/basicop/Duplicator.h">duplicator</a></td>
            <td>duplicate the input into multiple outputs</td>
        </tr>
        <tr>
            <td><a href="blocks/basicop/Splitter.h">splitter</a></td>
            <td>split a N dimmention vector to scalar which goes out to
                N output blocks</td>
        </tr>
        <tr>
            <td><a href="blocks/basicop/Transpose.h">transpose</a></td>
            <td>compute 2D matrix transpose</td>
        </tr>
    </tbody>
</table>

### Converters - Convertion Blocks

<table>
    <thead>
        <tr>
            <th>Block</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><a href="blocks/converters/OrdinalConverter.h">ordinal converter</a></td>
            <td>learn automatically and convert from string to integer,
                where the id are automatically generated based on
                inputs and continuously increases as new text strings
                are encountered. It takes simple c++ string as input and
                unsigned long as output (so its a MixedPort).</td>
        </tr>
        <tr>
            <td><a href="blocks/converters/CharOrdinalConverter.h">char ordinal converter</a></td>
            <td>It does exactly same as "ordinal converter" but takes
                raw character as input instead of std::string. This is
                particularly useful when working with modules which output
                raw character as output (for example the word splitter).</td>
        </tr>
        <tr>
            <td><a href="blocks/converters/BasicTypeConverter.h">basic-type converter</a></td>
            <td>convert from one base type to another</td>
        </tr>
        <tr>
            <td><a href="blocks/converters/ToMatrixConverter.h">to-matrix converter</a></td>
            <td>convert scalar or vector input to matrix output but retaining the dimensions.
                This is particularly useful when a block needs an input in matrix
                format. Note that the data is not transformed, just that the dimensions
                are modified to be of the matrix type.</td>
        </tr>
    </tbody>
</table>


### Filters - Mathematical Filter Blocks

<table>
    <thead>
        <tr>
            <th>Block</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><a href="blocks/filter/Delay.h">delay</a></td>
            <td>with some initial condition which is the initial output
                till delay is reached for first output</td>
        </tr>
        <tr>
            <td><a href="blocks/filter/DownSample.h">downsample</a></td>
            <td>filter data to down-sample input stream of data</td>
        </tr>
    </tbody>
</table>

### Math - Basic Mathematical Blocks

<table>
    <thead>
        <tr>
            <th>Block</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><a href="blocks/math/RunningAverage.h">running-average</a></td>
            <td>an infinite running average of input (no over/underflow checking)</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Average.h">average</a></td>
            <td>an average of inputs (no over/underflow checking)</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Gain.h">gain</a></td>
            <td>multiply input with a constant factor</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Product.h">product</a></td>
            <td>multiply two input values</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Sum.h">sum</a></td>
            <td>add two input values</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Difference.h">difference</a></td>
            <td>difference between two input values</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Arccosh.h">arccosh</a></td>
            <td>inverse hyperbolic cosine</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Arcsinh.h">arcsine</a></td>
            <td>inverse hyperbolic sine</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Arctanh.h">arctanh</a></td>
            <td>inverse hyperbolic tan</td>
        </tr>
        <tr>
            <td><a href="blocks/math/ConstPow.h">constant power</a></td>
            <td>raise the input to a constant power</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Cos.h">cos</a></td>
            <td>cosine</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Expm1.h">expm1</a></td>
            <td>it computes \exp(x)-1</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Log1p.h">log1p</a></td>
            <td>it computes \log(1+x) </td>
        </tr>
        <tr>
            <td><a href="blocks/math/Sin.h">sin</a></td>
            <td>sine</td>
        </tr>
        <tr>
            <td><a href="blocks/math/Tan.h">tan</a></td>
            <td>tan</td>
        </tr>
    </tbody>
</table>

### Sinks - The Consumer Blocks

<table>
    <thead>
        <tr>
            <th>Block</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><a href="blocks/sinks/ArchiverSink.h">archive sink</a></td>
            <td>archives all the data without dropping any one. It is
                important to note that there is no bound checking, so
                you will quickly run out of memory for a very
                large data set.</td>
        </tr>
        <tr>
            <td><a href="blocks/sinks/OstreamSink.h">output stream sink</a></td>
            <td>a sink which can be used to store to a file or any c++ ostream</td>
        </tr>
        <tr>
            <td><a href="blocks/sinks/SocketSink.h">socket sink</a></td>
            <td>output to a socket and send it to remote system (udp or tcp)</td>
        </tr>
        <tr>
            <td><a href="blocks/sinks/StdoutSink.h">standard output</a></td>
            <td>dump the data (signal) on the standard output or the console</td>
        </tr>
        <tr>
            <td><a href="blocks/sinks/JsonDataExtractableSink.h">json data extractable sink</a></td>
            <td>a special sink to convert input signal to a json (text) data stream,
                which can then be attached to a websocket server to stream them
                to connected clients. There is a cool demo also added which
                demonstrates its usefulness via an html5 app to monitor live
                stream of signal (inside a virtual oscilloscope). This block
                stores a limited history of data, while dropping older
                events beyond the configured threshold.</td>
        </tr>
        <tr>
            <td><a href="blocks/sinks/Terminator.h">terminator</a></td>
            <td>a null port (or sort of a ground) which is useful when
                you dont want to look at a signal but since all the
                input and output must be connected so this must be
                used in such cases.</td>
        </tr>
    </tbody>
</table>

### Sources - The Generator Blocks

<table>
    <thead>
        <tr>
            <th>Block</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><a href="blocks/sources/CsvFileSource.h">csv file source</a></td>
            <td>source of csvfile and generates a vector output of string data
                type</td>
        </tr>
        <tr>
            <td><a href="blocks/sources/ConstantSource.h">constant</a></td>
            <td>source of constant number</td>
        </tr>
        <tr>
            <td><a href="blocks/sources/AudioSource.h">audio source</a></td>
            <td>read audio file (multiple non-patented) formats and generate signal stream</td>
        </tr>
        <tr>
            <td><a href="blocks/sources/ComplexStreamSource.h">complex stream source</a></td>
            <td>reads {I, Q} complex data from a stream and pass on as-is</td>
        </tr>
        <tr>
            <td><a href="blocks/sources/IstreamSource.h">input stream source</a></td>
            <td>a source which can read from any c++ istream source</td>
        </tr>
        <tr>
            <td><a href="blocks/sources/LinearSource.h">linear</a></td>
            <td>generates a linear signal based on an initial value and an increment per tick</td>
        </tr>
        <tr>
            <td><a href="blocks/sources/PulseSource.h">pulse</a></td>
            <td>generates a pulse signal of high and low values</td>
        </tr>
        <tr>
            <td><a href="blocks/sources/RandomSource.h">random</a></td>
            <td>generates a random signal source based on the c psuedo random number generator</td>
        </tr>
        <tr>
            <td><a href="blocks/sources/SelectiveCsvFileSource.h">selective csv file source</a></td>
            <td>selectively read specific columns from a text csv file and generate appropriate
                signal streams</td>
        </tr>
        <tr>
            <td><a href="blocks/sources/SocketSource.h">socket</a></td>
            <td>reads packets from socket and sends them out as a source. This can be
                used to receive data from remote location and feed to your subsystem.
                At present it supports tcp and udp sockets.</td>
        </tr>
        <tr>
            <td><a href="blocks/sources/StepSource.h">step</a></td>
            <td>generates a function which starts with an initial value and always increase
                by 1/td>
        </tr>
        <tr>
            <td><a href="blocks/sources/LinuxIOSource.h">step</a></td>
            <td>Allows reading from Linux raw IO port.</td>
        </tr>
    </tbody>
</table>


### Text - Text Data Processing

> Note that unless mentioned otherwise the following blocks deal with
> ascii characters only (no unicode unless specifically mentioned).

<table>
    <thead>
        <tr>
            <th>Block</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><a href="blocks/text/SplitWordsBlock.h">split-words</a></td>
            <td>Take a single character (scalar) or multiple characters
                (vector) as input and spit vector output where each
                one is a separate ascii word.
            </td>
        </tr>
    </tbody>
</table>



## Design Choices

### Flexible Input and Output Data

Each of the block is written such that it can receive either scalar, vector
or matrix data and mostly (unless there is a strong reason against it)
it computes over it. Additionally, the same block instance can receive different
type of data in its lifetime as well. There is no hard requirements against
it and not configuration option is provided to enforce it as well. This design
choice is made to allow complex scenarios (where changing dimension is required)
and avoid configuration. Instead this design lets the system learn the data
dimension automatically. The downside of this approach is that technically
the block can receive data of different dimensions (scalar, vector or matrix)
without any validation and needs to accept as it receives it. In case a
specific block cannot work in this manner then appropriate errors or assertions
should be enforced (but that is on a case-by-case basis).

At present there is no restriction on the dimensions of data passed each time
to the same block, while this is a lot flexible but can be an issue for an
incorrectly written block. (hint: see Buffer block)


## The Future

There are number of items and which are in the pipeline and mostly
I have it documented in my personal notes. The most important of them
is to improve the code coverage of the unit tests
and any new feature. Having said that there will be new features available
from time to to time.

## Thanks

Thanks for evaluating this project and hope you find it useful.
Feel free to create issues for bugs or new features.

C++ code can be daunting for newbies, but I strive to make the framework API
simple to understand and use. Having said that
it is important to pick up any good book on c++ which covers c++11 to
make the best use this framework. This will also allow you to avoid spending a
lot of time debugging language usage issues. A plethora of open source
frameworks allow an ever expanding functionality (on top of this framework)
with practically limitless possibilities.


## Authors

* Neeraj Sharma {[github: neeraj9](https://github.com/neeraj9)}

