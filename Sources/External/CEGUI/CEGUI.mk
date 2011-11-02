CEGUI_DIR:=$(BUILD)/CEGUI-0.7.5
CEGUI_LIB:=$(CEGUI_DIR)/cegui/src/.libs/libCEGUIBase.so
CEGUI_CONFIGURE:=$(CEGUI_DIR)/configure
CEGUI_PACKAGE:=CEGUI-0.7.5.tar.gz
CEGUI_PACKAGE_BUILD:=$(BUILD)/$(CEGUI_PACKAGE)

.PHONY: CEGUI
CEGUI: $(CEGUI_LIB) CopyLibraries

.PHONY: CopyLibraries 
CopyLibraries: $(CEGUI_LIB)
	@find $(CEGUI_DIR) -name "*.so" -exec cp -lf {} $(BUILD)/ \;

$(CEGUI_LIB): $(CEGUI_DIR)/config.status
	cd $(CEGUI_DIR) && make -j2 -s

$(CEGUI_DIR)/config.status: $(CEGUI_CONFIGURE)
	cd $(CEGUI_DIR) &&\
	./configure \
		--disable-samples \
		--disable-irrlicht-renderer \
		--disable-opengl-renderer \
		--disable-dependency-tracking \
		--disable-null-renderer \
		--disable-tga \
		--disable-stb \
		--disable-libxml

$(CEGUI_CONFIGURE): $(CEGUI_PACKAGE_BUILD)
	cd $(BUILD) && tar --touch -xf $(CEGUI_PACKAGE_BUILD) 

$(CEGUI_PACKAGE_BUILD): $(CEGUI_PACKAGE)
	mkdir -p $(BUILD)
	@echo cp $(CEGUI_PACKAGE) $(CEGUI_PACKAGE_BUILD)
	cp $(CEGUI_PACKAGE) $(CEGUI_PACKAGE_BUILD)


