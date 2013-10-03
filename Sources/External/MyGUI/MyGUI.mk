TARGET:=MyGUI
TYPE:=custom

PACKAGE_NAME=$(TARGET_BASE)/MyGUI.tar.gz
MYGUI_DIR=$(BUILD)/MyGUI

.PHONY: MyGUI
MyGUI: $(BUILD)/libMyGUIEngine.so

$(BUILD)/libMyGUIEngine.so: $(BUILD)/MyGUI/lib/libMyGUIEngine.so
	cp -v $(BUILD)/MyGUI/lib/* $(BUILD)

$(MYGUI_DIR)/lib/libMyGUIEngine.so:
	mkdir -p $(MYGUI_DIR)
	cd $(MYGUI_DIR) && \
		tar xf $(PACKAGE_NAME) && \
		cmake . && \
		make -j1 MyGUIEngine MyGUI.OgrePlatform

