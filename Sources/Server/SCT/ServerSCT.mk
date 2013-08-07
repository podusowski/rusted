TYPE:=application

SOURCES:=mainSCT.cpp
SOURCES+=Core/Component.cpp
SOURCES+=Core/Connection.cpp

SOURCES+=ServerVersionSct.cpp
SOURCES+=UserSct.cpp
SOURCES+=ShipSct.cpp
SOURCES+=ObjectsSct.cpp
SOURCES+=ActionsSct.cpp
SOURCES+=SmokeSct.cpp

CFLAGS+=-Wall -pedantic -g 
CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../../
CFLAGS+=-I$(TARGET_BASE)/../../Common
CFLAGS+=-I$(TARGET_BASE)/../../External 
CFLAGS+=-I$(TARGET_BASE)/../../External/GoogleTest
CFLAGS+=-I/usr/include/libxml2

LDFLAGS=-L. -L$(BUILD) 
LDFLAGS+=-lCommon -lCake -lpthread
LDFLAGS+=-lgtest-static

RESOURCES+=SctDataBase.sqlite3

DEPENDENCIES+=Server
DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a

$(TARGET_BASE)/SctDataBase.sqlite3: $(TARGET_BASE)/../DataBaseSchema.sql $(TARGET_BASE)/SctDataBase.sql
	@rm -f $@
	cat $^ | sqlite3 $@
