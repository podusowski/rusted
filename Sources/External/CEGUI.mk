.PHONY: CEGUI
CEGUI: Deploy
Deploy: CEGUI-0.7.5/cegui/src/.libs/libCEGUIBase.so

CEGUI-0.7.5/cegui/src/.libs/libCEGUIBase.so:
	cd CEGUI-0.7.5 &&\
	test -e config.status || ./configure \
		--disable-samples \
		--disable-irrlicht-renderer \
		--disable-opengl-renderer \
		--disable-dependency-tracking \
		--disable-null-renderer \
		--disable-tga \
		--disable-stb \
		--disable-libxml \
		&&\
	make -j2 -s

.PHONY: Deploy
Deploy:
	@echo "copying external libraries... "
	@mkdir -p $(BUILD)/CEGUI
	@find ../External/ -name "*.so" -exec cp -lf {} $(BUILD)/ \;

