TARGET:=CakeUT
TYPE:=application

CFLAGS+=-I$(TARGET_BASE)
LDFLAGS+=-L. -L$(BUILD) -lCommon -lCake -lpthread -lgtest -lgmock

SOURCES+=Serialization/UnitTests/TmsTest.cpp
SOURCES+=mainUT.cpp

DEPENDENCIES+=libCake.a

