TARGET:=CommonGoogleTestUT
TYPE:=application
		
SOURCES+=UnitTests/mainGoogleTestUT.cpp
SOURCES+=Game/UnitTests/UniverseTest.cpp
SOURCES+=Game/Object/UnitTests/ShipTest.cpp
SOURCES+=Game/Object/UnitTests/ObjectFactoryTest.cpp
SOURCES+=Game/UnitTests/UniverseLoaderTest.cpp
SOURCES+=Game/UnitTests/RustedTimeTest.cpp
SOURCES+=Game/UnitTests/TimeValueTest.cpp
SOURCES+=DataBase/UnitTests/DataBaseModuleTest.cpp
SOURCES+=DataBase/UnitTests/DataBaseNodeTest.cpp
SOURCES+=DataBase/UnitTests/XmlDataProviderTest.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External 
CFLAGS+=-I/usr/include/libxml2

LDFLAGS=-L. -L$(BUILD) -lCommon -lCake -lpthread -lboost_system -lxml2 -lgtest -lgmock

DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a
