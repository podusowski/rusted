APPNAME=Server

LDFLAGS=-L. -L$(BUILD) -lServer -lCommon -lCake -lpthread -lboost_system -lxml2

$(APPNAME): $(BUILD)/Server.cfg $(BUILD)/TestDataBase.xml $(BUILD)/runTestServer

$(BUILD)/Server.cfg: Server.cfg
	cp $< $@

$(BUILD)/TestDataBase.xml: TestDataBase.xml
	cp $< $@

$(BUILD)/runTestServer: runTestServer
	cp $< $@

DEPENDENCIES+=libServer.a libCommon.a libCake.a

include Makefile.leaf
