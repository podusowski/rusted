TARGET:=libServer.a
TYPE:=static_library

SOURCES= \
		main.cpp \
	    Network/Connection.cpp \
	    Network/ServerController.cpp \
	    Services/ServerInfoService.cpp \
	    Services/AuthorizationService.cpp \
        Services/EntityService.cpp \
	    Services/PlayerService.cpp \
	    Services/StaticObjectsService.cpp \
	    Services/RustedTimeService.cpp \
	    Services/ServiceDeployment.cpp \
        Game/Player.cpp \
        Game/PlayerContainer.cpp \

CFLAGS+=-Wall -pedantic -g 
CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/..
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External
CFLAGS+=-I/usr/include/libxml2
