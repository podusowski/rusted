APPNAME=Client

SOURCES= \
    main.cpp \
    Engine/Application.cpp \
    Engine/Engine.cpp \
    Graphics/Graphics.cpp \
	Gui/Gui.cpp \
	Input/Input.cpp \
	Network/Connection.cpp \
	Game/EntityContainer.cpp \
	Game/PlayerInfo.cpp \
	States/StateManager.cpp \
	States/StateDeployment.cpp \
    States/LoginState.cpp \
    States/EntitySelectState.cpp \
    States/PilotState.cpp \
	Services/RustedTimeService.cpp \
	Services/AuthorizationService.cpp \
	Services/EntityService.cpp \
	Services/StaticObjectService.cpp \
	Views/StaticObjectView.cpp \
	Views/PilotView.cpp \
	Graphics/OgreObject.cpp \

LDFLAGS= \
	-L$(BUILD) \
	-L../External/CEGUI-0.7.5/cegui/src/.libs/ \
	-L../External/CEGUI-0.7.5/cegui/src/RendererModules/Ogre/.libs/ \
	-lOgreMain \
	-lCEGUIBase \
	-lCEGUIOgreRenderer \
	-lOIS \
	-lCommon \
	-lpthread \
	-lboost_system \
	-lxml2

CFLAGS+=-I../ -I. -I../Common 
CFLAGS+=-I/usr/include/libxml2
CFLAGS+=-I/usr/include/OGRE
CFLAGS+=-I$(BUILD)/CEGUI-0.7.5/cegui/include/RendererModules/Ogre
CFLAGS+=-I$(BUILD)/CEGUI-0.7.5/cegui/include/

DEPENDENCIES+=libCommon.a CEGUI

$(APPNAME): $(BUILD)/runClient $(BUILD)/runTestClient

include Makefile.leaf

$(BUILD)/runClient: runClient 
	cp $< $@

$(BUILD)/runTestClient: runTestClient
	cp $< $@
