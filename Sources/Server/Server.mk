TARGET:=Server
TYPE:=application

LDFLAGS:=-L$(BUILD) -lServer -lCommon -lCake -lpthread -lboost_system -lxml2

RESOURCES:=Server.cfg TestDataBase.xml ShipClasses.xml runTestServer
DEPENDENCIES+=libServer.a libCommon.a libCake.a
