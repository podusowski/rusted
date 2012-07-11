TARGET:=Client
TYPE:=application

SOURCES+=main.cpp

LDFLAGS+=-L$(BUILD)
LDFLAGS+=-L$(TARGET_BASE)/../External/CEGUI-0.7.5/cegui/src/.libs/
LDFLAGS+=-L../External/CEGUI-0.7.5/cegui/src/RendererModules/Ogre/.libs/
LDFLAGS+=-lClient
LDFLAGS+=-lOgreMain
LDFLAGS+=-lCEGUIBase
LDFLAGS+=-lCEGUIOgreRenderer
LDFLAGS+=-lOIS
LDFLAGS+=-lCommon
LDFLAGS+=-lCake
LDFLAGS+=-lpthread
LDFLAGS+=-lboost_system
LDFLAGS+=-lxml2
LDFLAGS+=-lMyGUIEngine
LDFLAGS+=-lMyGUI.OgrePlatform

CFLAGS+=-I$(TARGET_BASE) 
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common 
CFLAGS+=-I$(TARGET_BASE)/../External
CFLAGS+=-I/usr/include/libxml2
CFLAGS+=-I/usr/include/OGRE
CFLAGS+=-I$(BUILD)/CEGUI-0.7.5/cegui/include/RendererModules/Ogre
CFLAGS+=-I$(BUILD)/CEGUI-0.7.5/cegui/include/
CFLAGS+=-I$(BUILD)/MyGUI/MyGUIEngine/include/
CFLAGS+=-I$(BUILD)/MyGUI/Platforms/Ogre/OgrePlatform/include/

DEPENDENCIES+=libClient.a libCommon.a libCake.a MyGUI CEGUI Contents

RESOURCES+=runClient
RESOURCES+=runTestClient

