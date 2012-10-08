TARGET:=libClient.a
TYPE:=static_library

SOURCES+=Engine/Engine.cpp
SOURCES+=Graphics/Graphics.cpp
SOURCES+=Graphics/Effects.cpp
SOURCES+=Graphics/MovingMeshEffect.cpp
SOURCES+=Gui/Gui.cpp
SOURCES+=Input/Input.cpp
SOURCES+=Input/OgreObjectRaycaster.cpp
SOURCES+=Network/Connection.cpp
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
SOURCES+=Views/Camera.cpp
SOURCES+=Views/Object.cpp
SOURCES+=Views/PilotView.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../ 
CFLAGS+=-I$(TARGET_BASE)/../Common 
CFLAGS+=-I$(TARGET_BASE)/../External
CFLAGS+=-I/usr/include/libxml2
CFLAGS+=-I/usr/include/OGRE
CFLAGS+=-I$(BUILD)/MyGUI/MyGUIEngine/include/
CFLAGS+=-I$(BUILD)/MyGUI/Platforms/Ogre/OgrePlatform/include/

DEPENDENCIES+=libCommon.a MyGUI

