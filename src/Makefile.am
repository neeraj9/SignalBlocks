# (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
# see LICENSE for license
#
LDADD = $(LOCAL_LIBS)
LOCAL_LIBS = \
 libsrc.la \
 ../blocks/libblocks.la \
 ../common/libcommon.la \
 ../common/socket/libsocketcommon.la

noinst_LTLIBRARIES = libsrc.la

libsrc_la_SOURCES = \
 sample.cc

bin_PROGRAMS = main

main_SOURCES = main.cc
