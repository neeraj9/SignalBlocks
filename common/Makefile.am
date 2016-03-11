# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
# see LICENSE for license
SUBDIRS = codec socket plugins

LDADD = $(LOCAL_LIBS)
LOCAL_LIBS = \
 libcommon.la

noinst_LTLIBRARIES = libcommon.la

libcommon_la_SOURCES = \
 ComplexType.cc \
 Lock.cxx \
 MixedPort.cc \
 Mutex.cxx \
 Port.cc \
 TimeTick.cc
