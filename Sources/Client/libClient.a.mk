TARGET:=libClient.a
TYPE:=static_library

SOURCES+=Engine/Engine.cpp
SOURCES+=Graphics/Graphics.cpp
SOURCES+=Graphics/Raycast.cpp
SOURCES+=Gui/Gui.cpp
SOURCES+=Input/Input.cpp
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
SOURCES+=Services/EffectsService.cpp
SOURCES+=Views/ObjectsView.cpp
SOURCES+=Views/Camera.cpp
SOURCES+=Views/PilotView.cpp
SOURCES+=Views/ActionsView.cpp
SOURCES+=Visuals/VisualObject.cpp
SOURCES+=Visuals/VisualUniverse.cpp
SOURCES+=Visuals/Effects.cpp
SOURCES+=Visuals/MovingMeshEffect.cpp
SOURCES+=Visuals/Explosion.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../ 
CFLAGS+=-I$(TARGET_BASE)/../Common
CFLAGS+=-I$(TARGET_BASE)/../External
CFLAGS+=-I/usr/include/OGRE
CFLAGS+=-I$(BUILD)/MyGUI/MyGUIEngine/include/
CFLAGS+=-I$(BUILD)/MyGUI/Platforms/Ogre/OgrePlatform/include/

DEPENDENCIES+=libCommon.a MyGUI

