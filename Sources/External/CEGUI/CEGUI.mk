TARGET:=CEGUI
TYPE:=custom

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

$(CEGUI_LIB): $(TARGET_BASE)/$(CEGUI_PACKAGE)
	mkdir -p $(CEGUI_DIR)
	cp $(TARGET_BASE)/$(CEGUI_PACKAGE) $(CEGUI_PACKAGE_BUILD)
	cp $(TARGET_BASE)/*.patch $(CEGUI_DIR)
	cd $(BUILD) && tar -xf $(CEGUI_PACKAGE_BUILD) 
	cd $(CEGUI_DIR) && \
	cat *.patch | patch -p0 && \
	./configure \
		--disable-samples \
		--disable-irrlicht-renderer \
		--disable-opengl-renderer \
		--disable-dependency-tracking \
		--disable-null-renderer \
		--disable-tga \
		--disable-stb \
		--disable-libxml \
	&& make -j2 -s

