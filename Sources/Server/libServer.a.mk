TARGET:=libServer.a
TYPE:=static_library

SOURCES:=main.cpp
SOURCES+=Network/Connection.cpp
SOURCES+=Network/ConnectionContext.cpp
SOURCES+=Network/ServerController.cpp
SOURCES+=Services/ServerInfoService.cpp
SOURCES+=Services/AuthorizationService.cpp
SOURCES+=Services/EntityService.cpp
SOURCES+=Services/PlayerService.cpp
SOURCES+=Services/RustedTimeService.cpp
SOURCES+=Services/ServiceDeployment.cpp
SOURCES+=Game/Player.cpp
SOURCES+=Game/PlayerContainer.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/..
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External
CFLAGS+=-I/usr/include/libxml2

DEPENDENCIES+=libCommon.a
