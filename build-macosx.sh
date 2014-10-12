#!/bin/sh

autoreconf -ivfs
ARCHFLAGS=-Wno-error=unused-command-line-argument-hard-error-in-future ./configure
make

