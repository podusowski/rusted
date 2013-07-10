TARGET:=ServerUT
TYPE:=application

SOURCES+=../Common/UnitTests/mainUT.cpp
SOURCES+=Game/Actions/UnitTests/AttackTest.cpp
SOURCES+=Game/Actions/UnitTests/BuildShipTest.cpp
SOURCES+=Game/Actions/UnitTests/ActionFactoryTest.cpp
SOURCES+=Game/Actions/UnitTests/ActionPerformerTest.cpp
SOURCES+=DataBase/UnitTests/DataBaseNodeTest.cpp
SOURCES+=DataBase/UnitTests/XmlDataProviderTest.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External 
CFLAGS+=-I$(TARGET_BASE)/../External/GoogleTest
CFLAGS+=-I/usr/include/libxml2
CFLAGS+=-I$(BUILD)/SOCI/core

LDFLAGS=-L. -L$(BUILD) -lServer -lCommon -lCake -lpthread -lboost_system -lxml2 -lgtest-static -lsoci_core

DEPENDENCIES+=libServer.a
DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a
DEPENDENCIES+=libgtest-static.a
