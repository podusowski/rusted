LIBNAME=libServer.a

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
        Game/StaticObjectContainer.cpp \

CFLAGS=-Wall -pedantic -g -I../ -I. -I../Common -I/usr/include/libxml2

include Makefile.leaf
