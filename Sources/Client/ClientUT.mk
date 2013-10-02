TARGET:=ClientUT
TYPE:=application

SOURCES+=../Common/UnitTests/mainUT.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External
CFLAGS+=-I$(TARGET_BASE)/../External/GoogleTest
CFLAGS+=-I$(TARGET_BASE)/../External/boost_1_52_0/
CFLAGS+=-I/usr/include/libxml2

LDFLAGS+=-L. -L$(BUILD) -lpthread
LDFLAGS+=-lClient
LDFLAGS+=-lCommon
LDFLAGS+=-lCake
LDFLAGS+=-lgtest-static
LDFLAGS+=-lOgreMain
LDFLAGS+=-lOIS

DEPENDENCIES+=libClient.a
DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a
