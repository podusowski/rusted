TARGET:=libClient.a
TYPE:=static_library

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
SOURCES+=Services/PlayerActionService.cpp
SOURCES+=Services/ObjectService.cpp
SOURCES+=Views/ObjectsView.cpp
SOURCES+=Views/Object.cpp
SOURCES+=Views/PilotView.cpp
SOURCES+=Graphics/OgreObject.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../ 
CFLAGS+=-I$(TARGET_BASE)/../Common 
CFLAGS+=-I$(TARGET_BASE)/../External
CFLAGS+=-I/usr/include/libxml2
CFLAGS+=-I/usr/include/OGRE
CFLAGS+=-I$(BUILD)/CEGUI-0.7.5/cegui/include/RendererModules/Ogre
CFLAGS+=-I$(BUILD)/CEGUI-0.7.5/cegui/include/

DEPENDENCIES+=libCommon.a CEGUI

