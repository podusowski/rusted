TARGET:=ut_deepp
TYPE:=application

CFLAGS+=-DDEEPP_ENABLE_LOG
CFLAGS+=-I$(TARGET_BASE)/../..
LDFLAGS+=-lpthread -lgtest

SOURCES+=InjectTest.cpp
SOURCES+=FactoryInjectTest.cpp
SOURCES+=main.cpp

