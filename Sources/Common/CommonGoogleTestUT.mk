APPNAME=CommonGoogleTestUT
		
SOURCES+=UnitTests/mainGoogleTestUT.cpp
SOURCES+=Game/UnitTests/UniverseTest.cpp
SOURCES+=Game/Object/UnitTests/ShipTest.cpp
SOURCES+=Game/Object/UnitTests/ObjectFactoryTest.cpp

CFLAGS=-Wall -pedantic -g -I. -I.. -I/usr/include/libxml2
CFLAGS+=-I../External

LDFLAGS=-L. -L$(BUILD) -lCommon -lpthread -lboost_system -lxml2 -lgtest -lgmock

DEPENDENCIES+=libCommon.a

include Makefile.leaf

