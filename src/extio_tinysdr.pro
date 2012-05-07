QT           += network
TEMPLATE      = lib
CONFIG       += dll
TARGET        = ExtIO_TinySDR

HEADERS       = extio_tinysdr.h
SOURCES       = extio_tinysdr.cpp dllmain.cpp

#QMAKE_LFLAGS += -static-libgcc
#QMAKE_LFLAGS += -static-libgcc -enable-stdcall-fixup
#QMAKE_LFLAGS += -enable-stdcall-fixup
DEF_FILE     += extio_tinysdr.def

DLLDESTDIR   = ../test
