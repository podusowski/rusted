TARGET:=ClientUT
TYPE:=application

SOURCES+=../Common/UnitTests/mainGoogleTestUT.cpp


CFLAGS:=-Wall -pedantic -g
CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External 
CFLAGS+=-I/usr/include/libxml2

LDFLAGS+=-L. -L$(BUILD) -lpthread -lboost_system -lxml2 -lcppunit
LDFLAGS+=-lClient
LDFLAGS+=-lCommon
LDFLAGS+=-lCake
LDFLAGS+=-lgtest
LDFLAGS+=-lgmock
LDFLAGS+=-lOgreMain
LDFLAGS+=-lOIS
LDFLAGS+=-lboost_system

DEPENDENCIES+=libClient.a
DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a
