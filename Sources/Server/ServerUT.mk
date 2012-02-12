TARGET:=ServerUT
TYPE:=application

SOURCES=UnitTests/mainUT.cpp \
        Game/UnitTests/PlayerTest.cpp \
        Game/UnitTests/PlayerContainerTest.cpp \
		UnitTests/ConnectionStub.cpp \

CFLAGS=-Wall -pedantic -g
CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External 
CFLAGS+=-I/usr/include/libxml2

LDFLAGS=-L. -L$(BUILD) -lpthread -lboost_system -lxml2 -lcppunit -lServer -lCommon -lCake -lgtest -lgmock

DEPENDENCIES+=libServer.a
DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a
