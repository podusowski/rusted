CEGUI_DIR:=$(BUILD)/CEGUI-0.7.5
CEGUI_LIB:=$(CEGUI_DIR)/cegui/src/.libs/libCEGUIBase.so
CEGUI_CONFIGURE:=$(CEGUI_DIR)/configure
CEGUI_PACKAGE:=CEGUI-0.7.5.tar.gz

.PHONY: CEGUI
CEGUI: $(CEGUI_LIB) Deploy

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

$(CEGUI_LIB): $(CEGUI_DIR)/config.status
	cd $(CEGUI_DIR) && make -j2 -s

$(CEGUI_CONFIGURE): $(CEGUI_PACKAGE)
	@echo "unpacking CEGUI"
	cp $(CEGUI_PACKAGE) $(BUILD)
	cd $(BUILD) && tar xf $(CEGUI_PACKAGE) 

.PHONY: Deploy
Deploy:
	@echo "install CEGUI"
	@find $(CEGUI_DIR) -name "*.so" -exec cp -lf {} $(BUILD)/ \;

