LIBNAME=libCommon.a

SOURCES=Thread.cpp \
		RustedCodec/RustedAbstractCoder.cpp \
		RustedCodec/RustedAbstractDecoder.cpp \
		RustedCodec/SimpleWriteBuffer.cpp \
		RustedCodec/SimpleReadBuffer.cpp \
		RustedCodec/AsioReadBuffer.cpp \
		RustedCodec/AsioWriteBuffer.cpp \
		Configuration/Configuration.cpp \
        DataBase/DataBase.cpp \
        DataBase/DataBaseNode.cpp \
        DataBase/XmlDataProvider.cpp \
        DataBase/DataBaseFactory.cpp \
        Game/RustedTime.cpp \
	    Game/Utilities/PasswordHash.cpp \
	    Game/StaticObject.cpp
SOURCES+=Game/Universe.cpp
SOURCES+=Game/Object/ObjectFactory.cpp
SOURCES+=Game/Object/Ship.cpp
SOURCES+=Game/Object/StaticObject.cpp
SOURCES+=Game/UniverseLoader.cpp

CFLAGS+=-Wall -pedantic -g 
CFLAGS+=-I. 
CFLAGS+=-I.. 
CFLAGS+=-I/usr/include/libxml2
CFLAGS+=-I../External/

$(LIBNAME): Messages/Messages.hpp

Messages/Messages.hpp: Messages/Messages.xml genMessages.py
	./genMessages.py

include Makefile.leaf
