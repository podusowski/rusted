TARGET:=ut_deepp
TYPE:=application

CFLAGS+=-I../.. -DDEEPP_ENABLE_LOG
LDFLAGS+=-lpthread -lgtest

SOURCES+=InjectTest.cpp
SOURCES+=FactoryInjectTest.cpp
SOURCES+=main.cpp

