# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
# see LICENSE for license

# http://stackoverflow.com/questions/11438613/conditional-subdir-objects
AUTOMAKE_OPTIONS = subdir-objects

LDADD = $(LOCAL_LIBS)
LOCAL_LIBS = \
 libblocks.la

noinst_LTLIBRARIES = libblocks.la

libblocks_la_SOURCES = \
 basicop/Buffer.cc \
 basicop/Demultiplex.cc \
 basicop/Multiplex.cc \
 converters/BasicTypeConverter.cc \
 math/BinaryOperator.cc \
 math/NOperator.cc \
 python/PythonNOperator.cc \
 sinks/OstreamSink.cc \
 sinks/PlotSink.cc \
 sinks/SocketSink.cc \
 sinks/StdoutSink.cc \
 sources/ComplexStreamSource.cc \
 sources/ConstantSource.cc \
 sources/IstreamSource.cc \
 sources/RandomSource.cc \
 sources/SocketSource.cc \
 sources/StepSource.cc \
 transceivers/SocketTransceiver.cc
