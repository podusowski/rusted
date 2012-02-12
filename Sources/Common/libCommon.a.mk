TARGET:=libCommon.a
TYPE:=static_library

SOURCES=Thread.cpp \
		RustedCodec/RustedAbstractCoder.cpp \
		RustedCodec/RustedAbstractDecoder.cpp \
		RustedCodec/SimpleWriteBuffer.cpp \
		RustedCodec/SimpleReadBuffer.cpp \
		RustedCodec/AsioReadBuffer.cpp \
		RustedCodec/AsioWriteBuffer.cpp
SOURCES+=RustedCodec/CakeWriteBuffer.cpp
SOURCES+=RustedCodec/CakeReadBuffer.cpp
SOURCES+=Configuration/Configuration.cpp \
        DataBase/DataBase.cpp \
        DataBase/DataBaseNode.cpp \
        DataBase/XmlDataProvider.cpp \
        DataBase/DataBaseFactory.cpp \
        Game/RustedTime.cpp \
	    Game/Utilities/PasswordHash.cpp
SOURCES+=Game/Universe.cpp
SOURCES+=Game/Object/ObjectFactory.cpp
SOURCES+=Game/Object/Ship.cpp
SOURCES+=Game/Object/StaticObject.cpp
SOURCES+=Game/UniverseLoader.cpp

CFLAGS+=-Wall -pedantic -g 
CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/.. 
CFLAGS+=-I$(TARGET_BASE)/../External/
CFLAGS+=-I/usr/include/libxml2

$(TARGET): $(TARGET_BASE)/Messages/Messages.hpp

$(TARGET_BASE)/Messages/Messages.hpp: $(TARGET_BASE)/Messages/Messages.xml $(TARGET_BASE)/genMessages.py
	cd $(TARGET_BASE) && ./genMessages.py
