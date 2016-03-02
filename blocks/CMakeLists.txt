# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

set(BLOCKS_SOURCES
        basicop/Buffer.cc
        basicop/Buffer.hh
        basicop/Demultiplex.cc
        basicop/Demultiplex.hh
        basicop/Multiplex.cc
        basicop/Multiplex.hh
        basicop/Serialize.hh
        converters/BasicTypeConverter.cc
        converters/BasicTypeConverter.hh
        filters/DownSample.hh
        math/Arccosh.hh
        math/Arcsinh.hh
        math/Arctanh.hh
        math/BinaryOperator.cc
        math/BinaryOperator.hh
        math/ConstPow.hh
        math/Cos.hh
        math/Difference.hh
        math/Expm1.hh
        math/Gain.hh
        math/Log1p.hh
        math/NOperator.cc
        math/NOperator.hh
        math/Product.hh
        math/Sin.hh
        math/Sum.hh
        math/Tan.hh
        python/PythonNOperator.cc
        python/PythonNOperator.hh
        sinks/FileSinkCreator.hh
        sinks/OstreamSink.cc
        sinks/OstreamSink.hh
        sinks/PlotSink.cc
        sinks/PlotSink.hh
        sinks/SocketSink.cc
        sinks/SocketSink.hh
        sinks/SocketSinkCreator.hh
        sinks/StdoutSink.cc
        sinks/StdoutSink.hh
        sinks/Terminator.hh
        sources/ComplexStreamSource.cc
        sources/ComplexStreamSource.hh
        sources/ConstantSource.cc
        sources/ConstantSource.hh
        sources/FileSourceCreator.hh
        sources/IstreamSource.cc
        sources/IstreamSource.hh
        sources/RandomSource.cc
        sources/RandomSource.hh
        sources/ServerSocketSource.cc
        sources/ServerSocketSource.hh
        sources/SocketSource.cc
        sources/SocketSource.hh
        sources/SocketSourceCreator.hh
        sources/StepSource.cc
        sources/StepSource.hh
        transceivers/SocketTransceiver.cc
        transceivers/SocketTransceiver.hh
        transceivers/SocketTransceiverCreator.hh)

add_library(blocks STATIC ${BLOCKS_SOURCES})

target_include_directories(blocks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})