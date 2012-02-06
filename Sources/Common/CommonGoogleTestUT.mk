APPNAME=CommonGoogleTestUT
		
SOURCES+=UnitTests/mainGoogleTestUT.cpp
SOURCES+=Game/UnitTests/UniverseTest.cpp
SOURCES+=Game/Object/UnitTests/ShipTest.cpp
SOURCES+=Game/Object/UnitTests/ObjectFactoryTest.cpp
SOURCES+=Game/UnitTests/UniverseLoaderTest.cpp
SOURCES+=Game/UnitTests/RustedTimeTest.cpp
SOURCES+=Game/UnitTests/TimeValueTest.cpp

CFLAGS=-Wall -pedantic -g -I. -I.. -I/usr/include/libxml2
CFLAGS+=-I../External

LDFLAGS=-L. -L$(BUILD) -lCommon -lCake -lpthread -lboost_system -lxml2 -lgtest -lgmock

DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a

include Makefile.leaf

