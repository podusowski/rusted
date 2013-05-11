TARGET:=CommonUT
TYPE:=application

SOURCES+=UnitTests/mainUT.cpp
SOURCES+=Game/UnitTests/UniverseTest.cpp
SOURCES+=Game/UnitTests/PlayerTest.cpp
SOURCES+=Game/Object/UnitTests/ShipTest.cpp
SOURCES+=Game/Object/UnitTests/FlightTrajectoryTest.cpp
SOURCES+=Game/UnitTests/RustedTimeTest.cpp
SOURCES+=Game/UnitTests/TimeValueTest.cpp
SOURCES+=Game/Utilities/UnitTests/IdGeneratorTest.cpp
SOURCES+=Game/Utilities/UnitTests/PasswordHashTest.cpp
SOURCES+=Math/UnitTests/Point3Test.cpp
SOURCES+=Math/UnitTests/Bezier3Test.cpp
SOURCES+=Math/UnitTests/Matrix3Test.cpp
SOURCES+=Math/UnitTests/DegreeTest.cpp
SOURCES+=Math/UnitTests/KinematicParticleTest.cpp
SOURCES+=RustedCodec/UnitTests/SimpleBufferTest.cpp
SOURCES+=Messages/UnitTests/MessagesTest.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External 
CFLAGS+=-I/usr/include/libxml2

LDFLAGS=-L. -L$(BUILD) -lCommon -lCake -lpthread -lboost_system -lxml2 -lgtest -lgmock

DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a

$(TARGET): $(TARGET_BASE)/Messages/UnitTests/MessagesTest.cpp

$(TARGET_BASE)/Messages/UnitTests/MessagesTest.cpp: $(TARGET_BASE)/Messages/Messages.xml $(TARGET_BASE)/genMessages.py $(TARGET_BASE)/genMessagesUT.py
	@mkdir -p $(TARGET_BASE)/Messages/UnitTests
	cd $(TARGET_BASE) && ./genMessagesUT.py

