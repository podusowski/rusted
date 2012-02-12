TARGET:=Server
TYPE:=application

LDFLAGS:=-L$(BUILD) -lServer -lCommon -lCake -lpthread -lboost_system -lxml2

$(TARGET): $(BUILD) $(BUILD)/Server.cfg $(BUILD)/TestDataBase.xml $(BUILD)/runTestServer

$(BUILD)/Server.cfg: $(TARGET_BASE)/Server.cfg
	cp $< $@

$(BUILD)/TestDataBase.xml: $(TARGET_BASE)/TestDataBase.xml
	cp $< $@

$(BUILD)/runTestServer: $(TARGET_BASE)/runTestServer
	cp $< $@

DEPENDENCIES+=libServer.a libCommon.a libCake.a
