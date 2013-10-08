TARGET:=MyGUI
TYPE:=custom

PACKAGE_NAME=$(TARGET_BASE)/MyGUI_3.2.0_patched.tar.gz
MYGUI_DIR=$(BUILD)/MyGUI

.PHONY: MyGUI
MyGUI: $(BUILD)/libMyGUIEngine.so

$(BUILD)/libMyGUIEngine.so: $(BUILD)/MyGUI/lib/libMyGUIEngine.so
	cp -v $(MYGUI_DIR)/_build/lib/* $(BUILD)

$(MYGUI_DIR)/lib/libMyGUIEngine.so: OGRE
	mkdir -p $(MYGUI_DIR)
	cd $(MYGUI_DIR) && tar xf $(PACKAGE_NAME)
	mkdir -p $(MYGUI_DIR)/_build
	cd $(MYGUI_DIR)/_build && \
		cmake .. -DMYGUI_RENDERSYSTEM=2 -DOGRE_SOURCE=$(OGRE_DIR) -DOGRE_BUILD=$(OGRE_DIR)/_build && \
		make -j1 MyGUIEngine MyGUI.OgrePlatform

