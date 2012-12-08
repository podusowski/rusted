TARGET:=ServerUT
TYPE:=application

SOURCES+=../Common/UnitTests/mainGoogleTestUT.cpp
SOURCES+=Game/UnitTests/PlayerContainerTest.cpp
SOURCES+=Game/Actions/UnitTests/AttackTest.cpp
SOURCES+=Game/Actions/UnitTests/BuildShipTest.cpp
SOURCES+=Game/Actions/UnitTests/ActionFactoryTest.cpp
SOURCES+=Game/Actions/UnitTests/ActionPerformerTest.cpp
SOURCES+=Game/UnitTests/UniverseLoaderTest.cpp
SOURCES+=Game/UnitTests/ObjectFactoryTest.cpp
SOURCES+=Game/UnitTests/ShipClassTest.cpp
SOURCES+=Game/UnitTests/ShipClassContainerTest.cpp
SOURCES+=DataBase/UnitTests/DataBaseModuleTest.cpp
SOURCES+=DataBase/UnitTests/DataBaseNodeTest.cpp
SOURCES+=DataBase/UnitTests/XmlDataProviderTest.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External 
CFLAGS+=-I$(TARGET_BASE)/../External/GoogleTest
CFLAGS+=-I/usr/include/libxml2

LDFLAGS=-L. -L$(BUILD) -lServer -lCommon -lCake -lpthread -lboost_system -lxml2 -lgtest-static

DEPENDENCIES+=libServer.a
DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a
DEPENDENCIES+=libgtest-static.a
