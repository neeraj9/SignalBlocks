# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
# This file is part of SignalBlocks.
#
# SignalBlocks is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SignalBlocks is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with SignalBlocks.  If not, see <http://www.gnu.org/licenses/>.
#


set(BLOCKS_SOURCES
        basicop/Buffer.h
        basicop/Demultiplex.h
        basicop/Multiplex.h
        converters/BasicTypeConverter.cpp
        converters/BasicTypeConverter.h
        filters/DownSample.h
        math/Arccosh.h
        math/Arcsinh.h
        math/Arctanh.h
        math/BinaryOperator.h
        math/ConstPow.h
        math/Cos.h
        math/Difference.h
        math/Expm1.h
        math/Gain.h
        math/Log1p.h
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
        sinks/SocketSink.cpp
        sinks/SocketSink.h
        sinks/SocketSinkCreator.h
        sinks/StdoutSink.h
        sinks/Terminator.h
        sources/ComplexStreamSource.h
        sources/ConstantSource.h
        sources/FileSourceCreator.h
        sources/IstreamSource.h
        sources/RandomSource.h
        sources/ServerSocketSource.cpp
        sources/ServerSocketSource.h
        sources/SocketSource.cpp
        sources/SocketSource.h
        sources/SocketSourceCreator.h
        sources/StepSource.h
        sources/CsvFileSource.cpp
        sources/CsvFileSource.h
        sources/SelectiveCsvFileSource.h
        converters/OrdinalConverter.cpp
        converters/OrdinalConverter.h
        basicop/Duplicator.h
        basicop/Splitter.h
        math/UnaryOperator.h
        sinks/ArchiverSink.h
        sources/LinearSource.h
        sources/PulseSource.h
        filters/Delay.h
        sources/AudioSource.h
        sinks/JsonDataExtractableSink.h
        basicop/Transpose.h
        converters/ToMatrixConverter.h
        text/SplitWordsBlock.h
        text/SplitWordsBlock.cpp
        converters/CharOrdinalConverter.cpp
        converters/CharOrdinalConverter.h
        math/RunningAverage.h
        math/Average.h
        text/TextCsvParser.h
        text/TextCsvParser.cpp
        text/SplitLineBlock.cpp
        text/SplitLineBlock.h
        sources/LinuxIOSource.cpp
        sources/LinuxIOSource.h
        sources/LinuxGpioSource.cpp
        sources/LinuxGpioSource.h)

# conditionally include blocks for rtlsdr when the libraries
# are available, else dont build them
if (LIBRTLSDR_FOUND)
list(APPEND BLOCKS_SOURCES sources/RtlsdrSource.cpp sources/RtlsdrSource.h)
endif(LIBRTLSDR_FOUND)


add_library(blocks STATIC ${BLOCKS_SOURCES})

target_include_directories(blocks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
