LIBNAME=libClient.a

SOURCES+=Engine/Application.cpp
SOURCES+=Engine/Engine.cpp
SOURCES+=Graphics/Graphics.cpp
SOURCES+=Gui/Gui.cpp
SOURCES+=Input/Input.cpp
SOURCES+=Network/Connection.cpp
SOURCES+=Game/PlayerInfo.cpp
SOURCES+=States/StateManager.cpp
SOURCES+=States/StateDeployment.cpp
SOURCES+=States/LoginState.cpp
SOURCES+=States/EntitySelectState.cpp
SOURCES+=States/PilotState.cpp
SOURCES+=Services/RustedTimeService.cpp
SOURCES+=Services/AuthorizationService.cpp
SOURCES+=Services/EntityService.cpp
SOURCES+=Services/StaticObjectService.cpp
SOURCES+=Views/StaticObjectView.cpp
SOURCES+=Views/PilotView.cpp
SOURCES+=Graphics/OgreObject.cpp

CFLAGS+=-I../ -I. 
CFLAGS+=-I../Common 
CFLAGS+=-I../External
CFLAGS+=-I/usr/include/libxml2
CFLAGS+=-I/usr/include/OGRE
CFLAGS+=-I$(BUILD)/CEGUI-0.7.5/cegui/include/RendererModules/Ogre
CFLAGS+=-I$(BUILD)/CEGUI-0.7.5/cegui/include/

DEPENDENCIES+=libCommon.a CEGUI

include Makefile.leaf
