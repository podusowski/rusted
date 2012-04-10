TARGET:=ServerUT
TYPE:=application

SOURCES+=../Common/UnitTests/mainGoogleTestUT.cpp
SOURCES+=Game/UnitTests/PlayerContainerTest.cpp
SOURCES+=UnitTests/ConnectionStub.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External 
CFLAGS+=-I/usr/include/libxml2

LDFLAGS=-L. -L$(BUILD) -lpthread -lboost_system -lxml2 -lgtest -lgmock -lServer -lCommon -lCake 

DEPENDENCIES+=libServer.a
DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a
