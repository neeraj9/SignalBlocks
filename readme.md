# SigBlocks

[![Build Status](https://travis-ci.org/neeraj9/SignalBlocks.svg?branch=master)](https://travis-ci.org/neeraj9/SignalBlocks)
[![Software License](https://img.shields.io/badge/license-GPLv3-blue.svg?style=flat-square)](LICENSE)

The SignalBlocks is a c++11 framework to implement blocks of
logic which can be tied together like digital signal processing
(DSP) blocks (as in Matlab). This toolset aims to be fast, adaptable
and free (GPLv3 license). Although I initially intended this for the
student and research community, but this tool is generic enough to
be used elsewhere.

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

C++ code can be daunting for newbies, but I strive to make the framework API
simple to understand and use. Having said that
it is important to pick up any good book on c++ which covers c++11 to
make the best use this framework. This will also allow you to avoid spending a
lot of time debugging language usage issues. A plethora of open source
frameworks allow an ever expanding functionality (on top of this framework)
with practically limitless possibilities.


## A Sneak Peak

"The proof is in the pudding ..." and the following demo should get sufficient
information (and possibly motivation) to decide on this project.

Lets take a sneak peak at a sample c++ source which reads
audio from a file, apply a constant gain and then stream the output to a remote
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

## Environment Setup

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


## Motivation

I was always fascinated by Matlab's power and ease of use while in
college and wanted to implement something similar, but in a language
which can be compiled to native format for fast performance.
This project is an attempt to give a basic framework which will (soon)
encapsulate many of the open source software to construct a building
block for such requirement.

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
            <td>de-multiplexer</td>
            <td>one to many</td>
        </tr>
        <tr>
            <td>multiplexer</td>
            <td>many to one</td>
        </tr>
        <tr>
            <td>buffer</td>
            <td>convert serial data to a buffer of given size</td>
        </tr>
        <tr>
            <td>duplicator</td>
            <td>duplicate the input into multiple outputs</td>
        </tr>
        <tr>
            <td>splitter</td>
            <td>split a N dimmention vector to scalar which goes out to
                N output blocks</td>
        </tr>
        <tr>
            <td>transpose</td>
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
            <td>ordinal converter</td>
            <td>learn automatically and convert from string to integer,
                where the id are automatically generated based on
                inputs and continuously increases as new text strings
                are encountered. It takes simple c++ string as input and
                unsigned long as output (so its a MixedPort).</td>
        </tr>
        <tr>
            <td>basic-type converter</td>
            <td>convert from one base type to another</td>
        </tr>
        <tr>
            <td>to-matrix converter</td>
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
            <td>delay</td>
            <td>with some initial condition which is the initial output
                till delay is reached for first output</td>
        </tr>
        <tr>
            <td>downsample</td>
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
            <td>gain</td>
            <td>multiply input with a constant factor</td>
        </tr>
        <tr>
            <td>product</td>
            <td>multiply two input values</td>
        </tr>
        <tr>
            <td>sum</td>
            <td>add two input values</td>
        </tr>
        <tr>
            <td>difference</td>
            <td>difference between two input values</td>
        </tr>
        <tr>
            <td>arccosh</td>
            <td>inverse hyperbolic cosine</td>
        </tr>
        <tr>
            <td>arcsine</td>
            <td>inverse hyperbolic sine</td>
        </tr>
        <tr>
            <td>arctanh</td>
            <td>inverse hyperbolic tan</td>
        </tr>
        <tr>
            <td>constant power</td>
            <td>raise the input to a constant power</td>
        </tr>
        <tr>
            <td>cos</td>
            <td>cosine</td>
        </tr>
        <tr>
            <td>expm1</td>
            <td>it computes \exp(x)-1</td>
        </tr>
        <tr>
            <td>log1p</td>
            <td>it computes \log(1+x) </td>
        </tr>
        <tr>
            <td>sin</td>
            <td>sine</td>
        </tr>
        <tr>
            <td>tan</td>
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
            <td>archive sink</td>
            <td>archives all the data without dropping any one. It is
                important to note that there is no bound checking, so
                you will quickly run out of memory for a very
                large data set.</td>
        </tr>
        <tr>
            <td>output stream sink</td>
            <td>a sink which can be used to store to a file or any c++ ostream</td>
        </tr>
        <tr>
            <td>socket sink</td>
            <td>output to a socket and send it to remote system (udp or tcp)</td>
        </tr>
        <tr>
            <td>standard output</td>
            <td>dump the data (signal) on the standard output or the console</td>
        </tr>
        <tr>
            <td>json data extractable sink</td>
            <td>a special sink to convert input signal to a json (text) data stream,
                which can then be attached to a websocket server to stream them
                to connected clients. There is a cool demo also added which
                demonstrates its usefulness via an html5 app to monitor live
                stream of signal (inside a virtual oscilloscope). This block
                stores a limited history of data, while dropping older
                events beyond the configured threshold.</td>
        </tr>
        <tr>
            <td>terminator</td>
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
            <td>csv file source</td>
            <td>source of csvfile and generates a vector output of string data
                type</td>
        </tr>
        <tr>
            <td>constant</td>
            <td>source of constant number</td>
        </tr>
        <tr>
            <td>audio source</td>
            <td>read audio file (multiple non-patented) formats and generate signal stream</td>
        </tr>
        <tr>
            <td>complex stream source</td>
            <td>reads {I, Q} complex data from a stream and pass on as-is</td>
        </tr>
        <tr>
            <td>input stream source</td>
            <td>a source which can read from any c++ istream source</td>
        </tr>
        <tr>
            <td>linear</td>
            <td>generates a linear signal based on an initial value and an increment per tick</td>
        </tr>
        <tr>
            <td>pulse</td>
            <td>generates a pulse signal of high and low values</td>
        </tr>
        <tr>
            <td>random</td>
            <td>generates a random signal source based on the c psuedo random number generator</td>
        </tr>
        <tr>
            <td>selective csv file source</td>
            <td>selectively read specific columns from a text csv file and generate appropriate
                signal streams</td>
        </tr>
        <tr>
            <td>socket</td>
            <td>reads packets from socket and sends them out as a source. This can be
                used to receive data from remote location and feed to your subsystem.
                At present it supports tcp and udp sockets.</td>
        </tr>
        <tr>
            <td>step</td>
            <td>generates a function which starts with an initial value and always increase
                by 1/td>
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
is to improve the code coverage of the unit tests as must possible
(close to 100%) and subsequently move to any new feature. Having said
that there will be new features available from time to to time.

## Thanks

Feel free to create issues for bugs or new features.

Copyright (c) 2016 Neeraj Sharma.
