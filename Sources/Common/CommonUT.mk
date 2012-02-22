TARGET:=CommonUT
TYPE:=application
		
SOURCES=UnitTests/mainUT.cpp \
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
		Game/Utilities/UnitTests/PasswordHashTest.cpp \

CFLAGS+=-Wall -pedantic -g
CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External 
CFLAGS+=-I/usr/include/libxml2

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

$(TARGET): $(TARGET_BASE)/Messages/UnitTests/MessagesTest.cpp

$(TARGET_BASE)/Messages/UnitTests/MessagesTest.cpp: $(TARGET_BASE)/Messages/Messages.xml $(TARGET_BASE)/genMessages.py $(TARGET_BASE)/genMessagesUT.py
	@mkdir -p $(TARGET_BASE)/Messages/UnitTests
	cd $(TARGET_BASE) && ./genMessagesUT.py

