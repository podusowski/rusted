APPNAME=ServerSCT

SOURCES=SCT/mainSCT.cpp \
		SCT/Component.cpp \
		SCT/Connection.cpp \
        SCT/DataBaseUtils.cpp \
		SCT/SctServerVersion.cpp \
		SCT/SctUserAuthorization.cpp \
		SCT/SctBasicEntitiesOperations.cpp \
		SCT/SctStaticObjects.cpp

CFLAGS=-Wall -pedantic -g -I../ -I. -I../Common -I/usr/include/libxml2
LDFLAGS=-L. -L$(BUILD) -lCommon -lpthread -lboost_system -lxml2 -lcppunit

$(APPNAME): $(BUILD)/runServerSCT 

$(BUILD)/runServerSCT: runServerSCT
	$(COPY)

include Makefile.leaf
