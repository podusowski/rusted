LIBNAME=libCommon.a

libCommon_SOURCES=Thread.cpp \
		RustedCodec/RustedAbstractCoder.cpp \
		RustedCodec/RustedAbstractDecoder.cpp \
		RustedCodec/SimpleWriteBuffer.cpp \
		RustedCodec/SimpleReadBuffer.cpp \
		RustedCodec/AsioReadBuffer.cpp \
		RustedCodec/AsioWriteBuffer.cpp
libCommon_SOURCES+=RustedCodec/CakeWriteBuffer.cpp
libCommon_SOURCES+=RustedCodec/CakeReadBuffer.cpp
libCommon_SOURCES+=Configuration/Configuration.cpp \
        DataBase/DataBase.cpp \
        DataBase/DataBaseNode.cpp \
        DataBase/XmlDataProvider.cpp \
        DataBase/DataBaseFactory.cpp \
        Game/RustedTime.cpp \
	    Game/Utilities/PasswordHash.cpp
libCommon_SOURCES+=Game/Universe.cpp
libCommon_SOURCES+=Game/Object/ObjectFactory.cpp
libCommon_SOURCES+=Game/Object/Ship.cpp
libCommon_SOURCES+=Game/Object/StaticObject.cpp
libCommon_SOURCES+=Game/UniverseLoader.cpp

CFLAGS+=-Wall -pedantic -g 
CFLAGS+=-I. 
CFLAGS+=-I.. 
CFLAGS+=-I/usr/include/libxml2
CFLAGS+=-I../External/

$(LIBNAME): Messages/Messages.hpp

Messages/Messages.hpp: Messages/Messages.xml genMessages.py
	./genMessages.py

include Makefile.leaf
