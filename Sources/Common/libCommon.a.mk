TARGET:=libCommon.a
TYPE:=static_library

SOURCES+=RustedCodec/RustedAbstractCoder.cpp
SOURCES+=RustedCodec/RustedAbstractDecoder.cpp
SOURCES+=RustedCodec/SimpleWriteBuffer.cpp
SOURCES+=RustedCodec/SimpleReadBuffer.cpp
SOURCES+=RustedCodec/AsioReadBuffer.cpp
SOURCES+=RustedCodec/AsioWriteBuffer.cpp
SOURCES+=RustedCodec/CakeWriteBuffer.cpp
SOURCES+=RustedCodec/CakeReadBuffer.cpp
SOURCES+=Configuration/Configuration.cpp
SOURCES+=Game/RustedTime.cpp
SOURCES+=Game/Utilities/PasswordHash.cpp
SOURCES+=Game/Universe.cpp
SOURCES+=Game/Object/Ship.cpp
SOURCES+=Game/Object/StaticObject.cpp

CFLAGS+=-Wall -pedantic -g 
CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/.. 
CFLAGS+=-I$(TARGET_BASE)/../External/
CFLAGS+=-I/usr/include/libxml2

$(TARGET): $(TARGET_BASE)/Messages/Messages.hpp

$(TARGET_BASE)/Messages/Messages.hpp: $(TARGET_BASE)/Messages/Messages.xml $(TARGET_BASE)/genMessages.py
	cd $(TARGET_BASE) && ./genMessages.py
