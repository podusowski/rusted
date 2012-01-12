APPNAME=CommonUT
		
SOURCES=UnitTests/mainUT.cpp \
        UnitTests/ThreadTest.cpp \
        UnitTests/BezierTest.cpp \
        UnitTests/Point3Test.cpp \
        RustedCodec/UnitTests/SimpleBufferTest.cpp \
        RustedCodec/UnitTests/RustedAbstractCodecTest.cpp \
		Messages/UnitTests/MessagesTest.cpp \
		RustedCodec/UnitTests/AsioBufferTest.cpp \
		Configuration/UnitTests/ConfigurationTest.cpp \
        DataBase/UnitTests/DataBaseNodeTest.cpp \
        DataBase/UnitTests/XmlDataProviderTest.cpp \
        DataBase/UnitTests/DataBaseModuleTest.cpp \
        Game/UnitTests/RustedTimeTest.cpp \
		Game/Utilities/UnitTests/PasswordHashTest.cpp \

CFLAGS=-Wall -pedantic -g -I. -I.. -I/usr/include/libxml2

LDFLAGS=-L. -L$(BUILD) 
LDFLAGS+=-lCommon 
LDFLAGS+=-lCake
LDFLAGS+=-lpthread 
LDFLAGS+=-lboost_system 
LDFLAGS+=-lxml2 
LDFLAGS+=-lcppunit
LDFLAGS+=-lgtest
LDFLAGS+=-lgmock

DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a

Messages/UnitTests/MessagesTest.cpp: Messages/Messages.xml genMessages.py genMessagesUT.py
	@mkdir -p Messages/UnitTests
	./genMessagesUT.py

include Makefile.leaf

