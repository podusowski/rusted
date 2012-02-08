APPNAME=ServerSCT

SOURCES:=mainSCT.cpp
SOURCES+=Core/Component.cpp
SOURCES+=Core/Connection.cpp

SOURCES+=ServerVersionSct.cpp
SOURCES+=UserSct.cpp
SOURCES+=ShipSct.cpp
SOURCES+=StaticObjectsSct.cpp

CFLAGS+=-Wall -pedantic -g 
CFLAGS+=-I../.. 
CFLAGS+=-I.. 
CFLAGS+=-I.
CFLAGS+=-I../../Common
CFLAGS+=-I../../External
CFLAGS+=-I/usr/include/libxml2

LDFLAGS=-L. -L$(BUILD) 
LDFLAGS+=-lCommon -lCake -lpthread -lboost_system -lxml2
LDFLAGS+=-lgtest

RESOURCES+=SampleDataBase.xml

DEPENDENCIES+=Server
DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a

include Makefile.leaf
