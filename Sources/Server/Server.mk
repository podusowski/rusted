TARGET:=Server
TYPE:=application

LDFLAGS:=-L$(BUILD) -lServer -lCommon -lCake -lpthread -lboost_system -lxml2

RESOURCES:=Server.cfg TestDataBase.xml ShipClasses.xml runTestServer
DEPENDENCIES+=libServer.a libCommon.a libCake.a

$(TARGET): $(BUILD)/BigDataBase.xml

$(BUILD)/BigDataBase.xml: $(TARGET_BASE)/genBigDataBase.py
	$(TARGET_BASE)/genBigDataBase.py -o $@

