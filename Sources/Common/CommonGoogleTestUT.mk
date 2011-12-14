APPNAME=CommonGoogleTestUT
		
SOURCES+=UnitTests/mainGoogleTestUT.cpp
SOURCES+=Game/UnitTests/UniverseTest.cpp
SOURCES+=Game/Object/UnitTests/ShipTest.cpp

CFLAGS=-Wall -pedantic -g -I. -I.. -I/usr/include/libxml2
LDFLAGS=-L. -L$(BUILD) -lCommon -lpthread -lboost_system -lxml2 -lgtest

include Makefile.leaf

