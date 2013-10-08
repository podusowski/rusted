TARGET:=libCommon.a
TYPE:=static_library

SOURCES+=Game/RustedTime.cpp
SOURCES+=Game/TimeValue.cpp
SOURCES+=Game/Utilities/PasswordHash.cpp
SOURCES+=Game/Utilities/IdGenerator.cpp
SOURCES+=Game/Universe.cpp
SOURCES+=Game/Player.cpp
SOURCES+=Game/Object/ObjectBase.cpp
SOURCES+=Game/Object/FlightTrajectory.cpp
SOURCES+=Game/Object/Ship.cpp
SOURCES+=Game/Object/Asteroid.cpp
SOURCES+=Game/Object/CargoHold.cpp
SOURCES+=Math/Quaternion.cpp
SOURCES+=Math/Bezier3.cpp
SOURCES+=Math/Matrix3.cpp
SOURCES+=Math/KinematicParticle.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/.. 
CFLAGS+=-I$(TARGET_BASE)/../External/
CFLAGS+=-I$(TARGET_BASE)/../External/boost/
CFLAGS+=-I/usr/include/libxml2

$(TARGET): $(TARGET_BASE)/Messages/Messages.hpp

LIBCOMMON_PROTOC:=$(TARGET_BASE)/../External/Cake/Networking/Protocol/protoc.py
LIBCOMMON_PROTO_XML:=$(TARGET_BASE)/Messages/Messages.xml

$(TARGET_BASE)/Messages/Messages.hpp: $(LIBCOMMON_PROTO_XML) $(LIBCOMMON_PROTOC)
	$(LIBCOMMON_PROTOC) --output $@ --namespace Common::Messages $(LIBCOMMON_PROTO_XML)

