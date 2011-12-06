APPNAME=ServerUT

SOURCES=UnitTests/mainUT.cpp \
        Game/UnitTests/PlayerTest.cpp \
        Game/UnitTests/PlayerContainerTest.cpp \
		Services/UnitTests/PlayerServiceTest.cpp \
		UnitTests/ConnectionStub.cpp \
        Services/UnitTests/EntityServiceTest.cpp \

CFLAGS=-Wall -pedantic -g -I../ -I. -I../Common -I/usr/include/libxml2
LDFLAGS=-L. -L$(BUILD) -lpthread -lboost_system -lxml2 -lcppunit -lServer -lCommon

DEPENDENCIES+=libServer.a
DEPENDENCIES+=libCommon.a

include Makefile.leaf
