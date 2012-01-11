APPNAME=ServerUT

SOURCES=UnitTests/mainUT.cpp \
        Game/UnitTests/PlayerTest.cpp \
        Game/UnitTests/PlayerContainerTest.cpp \
		UnitTests/ConnectionStub.cpp \

CFLAGS=-Wall -pedantic -g -I../ -I. -I../Common -I../External -I/usr/include/libxml2
LDFLAGS=-L. -L$(BUILD) -lpthread -lboost_system -lxml2 -lcppunit -lServer -lCommon -lgtest -lgmock

DEPENDENCIES+=libServer.a
DEPENDENCIES+=libCommon.a

include Makefile.leaf
