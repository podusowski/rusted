TARGET:=CakeUT
TYPE:=application

CFLAGS+=-I.
CFLAGS+=-I..
CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/..
CFLAGS+=-I$(TARGET_BASE)/../GoogleTest/
CFLAGS+=-I$(TARGET_BASE)/../boost_1_54_0/

LDFLAGS+=-L.
LDFLAGS+=-L$(BUILD)
LDFLAGS+=-lCake
LDFLAGS+=-lgtest-static

SOURCES+=Serialization/UnitTests/TmsTest.cpp
SOURCES+=Serialization/UnitTests/FcTest.cpp
SOURCES+=Networking/UnitTests/ServerSocketPoolTest.cpp
SOURCES+=Networking/Protocol/UnitTests/ProtocolTest.cpp
SOURCES+=Utils/UnitTests/StringListTest.cpp
SOURCES+=mainUT.cpp

DEPENDENCIES+=libCake.a
DEPENDENCIES+=$(TARGET_BASE)/Networking/Protocol/UnitTests/SampleProtocol.hpp

PROTOC=$(TARGET_BASE)/Networking/Protocol/protoc.py
PROTO_XML=$(TARGET_BASE)/Networking/Protocol/UnitTests/SampleProtocol.xml

$(TARGET_BASE)/Networking/Protocol/UnitTests/SampleProtocol.hpp: $(PROTOC) $(PROTO_XML)
	$(PROTOC) --output $@ $(PROTO_XML) --namespace Cake::Networking::Protocol

