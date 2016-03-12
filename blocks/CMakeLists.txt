# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# All rights reserved.

set(BLOCKS_SOURCES
        basicop/Buffer.cpp
        basicop/Buffer.h
        basicop/Demultiplex.cpp
        basicop/Demultiplex.h
        basicop/Multiplex.cpp
        basicop/Multiplex.h
        basicop/Serialize.h
        converters/BasicTypeConverter.cpp
        converters/BasicTypeConverter.h
        filters/DownSample.h
        math/Arccosh.h
        math/Arcsinh.h
        math/Arctanh.h
        math/BinaryOperator.cpp
        math/BinaryOperator.h
        math/ConstPow.h
        math/Cos.h
        math/Difference.h
        math/Expm1.h
        math/Gain.h
        math/Log1p.h
        math/NOperator.cpp
        math/NOperator.h
        math/Product.h
        math/Sin.h
        math/Sum.h
        math/Tan.h
        python/PythonNOperator.cpp
        python/PythonNOperator.h
        sinks/FileSinkCreator.h
        sinks/OstreamSink.cpp
        sinks/OstreamSink.h
        sinks/PlotSink.cpp
        sinks/PlotSink.h
        sinks/SocketSink.cpp
        sinks/SocketSink.h
        sinks/SocketSinkCreator.h
        sinks/StdoutSink.cpp
        sinks/StdoutSink.h
        sinks/Terminator.h
        sources/ComplexStreamSource.cpp
        sources/ComplexStreamSource.h
        sources/ConstantSource.cpp
        sources/ConstantSource.h
        sources/FileSourceCreator.h
        sources/IstreamSource.cpp
        sources/IstreamSource.h
        sources/RandomSource.cpp
        sources/RandomSource.h
        sources/ServerSocketSource.cpp
        sources/ServerSocketSource.h
        sources/SocketSource.cpp
        sources/SocketSource.h
        sources/SocketSourceCreator.h
        sources/StepSource.cpp
        sources/StepSource.h
        transceivers/SocketTransceiver.cpp
        transceivers/SocketTransceiver.h
        transceivers/SocketTransceiverCreator.h basicop/TransparentForwarder.h)

add_library(blocks STATIC ${BLOCKS_SOURCES})

target_include_directories(blocks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})