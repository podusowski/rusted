TARGET:=ClientUT
TYPE:=application

SOURCES+=../Common/UnitTests/mainGoogleTestUT.cpp

SOURCES+=Services/UnitTests/EntityServiceTest.cpp

CFLAGS=-Wall -pedantic -g -I../ -I. -I../Common -I../External -I/usr/include/libxml2
LDFLAGS+=-L. -L$(BUILD) -lpthread -lboost_system -lxml2 -lcppunit
LDFLAGS+=-lClient
LDFLAGS+=-lCommon
LDFLAGS+=-lCake
LDFLAGS+=-lgtest
LDFLAGS+=-lgmock
LDFLAGS+=-lOgreMain
LDFLAGS+=-lCEGUIBase
LDFLAGS+=-lCEGUIOgreRenderer
LDFLAGS+=-lOIS
LDFLAGS+=-lboost_system

DEPENDENCIES+=libClient.a
DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a
