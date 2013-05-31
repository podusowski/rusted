TARGET:=Client
TYPE:=application

SOURCES+=main.cpp

LDFLAGS+=-L$(BUILD)
LDFLAGS+=-lClient
LDFLAGS+=-lOgreMain
LDFLAGS+=-lOIS
LDFLAGS+=-lCommon
LDFLAGS+=-lCake
LDFLAGS+=-lpthread
LDFLAGS+=-lxml2
LDFLAGS+=-lMyGUIEngine
LDFLAGS+=-lMyGUI.OgrePlatform

CFLAGS+=-I$(TARGET_BASE) 
CFLAGS+=-I$(TARGET_BASE)/../
CFLAGS+=-I$(TARGET_BASE)/../Common 
CFLAGS+=-I$(TARGET_BASE)/../External
CFLAGS+=-I/usr/include/libxml2
CFLAGS+=-I/usr/include/OGRE
CFLAGS+=-I$(BUILD)/MyGUI/MyGUIEngine/include/
CFLAGS+=-I$(BUILD)/MyGUI/Platforms/Ogre/OgrePlatform/include/

DEPENDENCIES+=libClient.a libCommon.a libCake.a MyGUI Contents

RESOURCES+=runClient
RESOURCES+=runTestClient

