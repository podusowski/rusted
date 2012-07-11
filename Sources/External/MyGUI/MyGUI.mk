TARGET:=MyGUI
TYPE:=custom

PACKAGE_NAME=MyGUI.tar.gz

.PHONY: MyGUI
MyGUI: $(BUILD)/libMyGUIEngine.so

$(BUILD)/libMyGUIEngine.so: $(BUILD)/MyGUI/lib/libMyGUIEngine.so
	cp -v $(BUILD)/MyGUI/lib/* $(BUILD)

$(BUILD)/MyGUI/lib/libMyGUIEngine.so:
	mkdir -p $(BUILD)/MyGUI
	cp $(TARGET_BASE)/$(PACKAGE_NAME) $(BUILD)/MyGUI
	cd $(BUILD)/MyGUI && tar xf $(PACKAGE_NAME) && \
		cmake . && \
		make -j$(JOBS) MyGUI.OgrePlatform
