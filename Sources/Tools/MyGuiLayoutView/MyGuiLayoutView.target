TARGET:=MyGuiLayoutView
TYPE:=application

SOURCES+=main.cpp

CFLAGS+=-I$(TARGET_BASE)
CFLAGS+=-I$(TARGET_BASE)/../..
CFLAGS+=-I$(TARGET_BASE)/../../Client
CFLAGS+=-I$(TARGET_BASE)/../../Common 
CFLAGS+=-I$(TARGET_BASE)/../../External
CFLAGS+=-I/usr/include/libxml2
CFLAGS+=-I/usr/include/OGRE
CFLAGS+=-I$(BUILD)/MyGUI/MyGUIEngine/include/
CFLAGS+=-I$(BUILD)/MyGUI/Platforms/Ogre/OgrePlatform/include/

LDFLAGS+=-L$(BUILD)
LDFLAGS+=-lClient
LDFLAGS+=-lOgreMain
LDFLAGS+=-lOIS
LDFLAGS+=-lCommon
LDFLAGS+=-lCake
LDFLAGS+=-lpthread
LDFLAGS+=-lMyGUIEngine
LDFLAGS+=-lMyGUI.OgrePlatform

DEPENDENCIES+=libClient.a MyGUI

RESOURCES+=showLayout

