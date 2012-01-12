APPNAME=Client

SOURCES+=main.cpp

LDFLAGS= \
	-L$(BUILD) \
	-L../External/CEGUI-0.7.5/cegui/src/.libs/ \
	-L../External/CEGUI-0.7.5/cegui/src/RendererModules/Ogre/.libs/ \
	-lClient \
	-lOgreMain \
	-lCEGUIBase \
	-lCEGUIOgreRenderer \
	-lOIS \
	-lCommon \
	-lCake \
	-lpthread \
	-lboost_system \
	-lxml2 

CFLAGS+=-I../ -I. 
CFLAGS+=-I../Common 
CFLAGS+=-I../External
CFLAGS+=-I/usr/include/libxml2
CFLAGS+=-I/usr/include/OGRE
CFLAGS+=-I$(BUILD)/CEGUI-0.7.5/cegui/include/RendererModules/Ogre
CFLAGS+=-I$(BUILD)/CEGUI-0.7.5/cegui/include/

DEPENDENCIES+=libClient.a libCommon.a libCake.a

RESOURCES+=runClient
RESOURCES+=runTestClient

include Makefile.leaf

