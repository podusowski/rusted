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

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External
CFLAGS+=-I$(TARGET_BASE)/../External/GoogleTest

LDFLAGS=-L. -L$(BUILD) -lCommon -lCake -lpthread -lgtest-static

DEPENDENCIES+=libCommon.a
DEPENDENCIES+=libCake.a
DEPENDENCIES+=libgtest-static.a

