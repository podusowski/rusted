APPNAME=ServerSCT

SOURCES=mainSCT.cpp \
		Core/Component.cpp \
		Core/Connection.cpp \
        Core/DataBaseUtils.cpp \
		SctServerVersion.cpp \
		SctUserAuthorization.cpp \
		SctBasicEntitiesOperations.cpp \
		SctStaticObjects.cpp

CFLAGS+=-Wall -pedantic -g 
CFLAGS+=-I../.. 
CFLAGS+=-I.. 
CFLAGS+=-I.
CFLAGS+=-I../../Common
CFLAGS+=-I/usr/include/libxml2

LDFLAGS=-L. -L$(BUILD) -lCommon -lpthread -lboost_system -lxml2 -lcppunit
LDFLAGS+=-lgtest

$(APPNAME): $(BUILD)/runServerSCT 

$(BUILD)/runServerSCT: runServerSCT
	$(COPY)

DEPENDENCIES+=Server
DEPENDENCIES+=libCommon.a

include Makefile.leaf
