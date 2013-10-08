TARGET:=AdministrationClient
TYPE:=application

SOURCES+=main.cpp

LDFLAGS+=-L$(BUILD)
LDFLAGS+=-lCommon
LDFLAGS+=-lCake

CFLAGS+=-I$(TARGET_BASE) 
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External

DEPENDENCIES+=libCommon.a libCake.a

