CEGUI_DIR=$(BUILD)/CEGUI-0.7.5
CEGUI_LIB=$(CEGUI_DIR)/cegui/src/.libs/libCEGUIBase.so
CEGUI_CONFIGURE=$(CEGUI_DIR)/configure
CEGUI_PACKAGE=CEGUI-0.7.5.tar.gz

.PHONY: CEGUI
CEGUI: $(CEGUI_LIB) 

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

$(CEGUI_CONFIGURE): CEGUI-0.7.5.tar.gz
	cp $(CEGUI_PACKAGE) $(BUILD)
	cd $(BUILD) && tar xvf $(CEGUI_PACKAGE) 

.PHONY: Deploy
Deploy:
	@echo "copying external libraries... "
	@mkdir -p $(BUILD)/CEGUI
	@find ../External/ -name "*.so" -exec cp -lf {} $(BUILD)/ \;

