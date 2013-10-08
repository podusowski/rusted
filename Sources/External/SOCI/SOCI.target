TARGET:=SOCI
TYPE:=custom

SOCI_PACKAGE_NAME:=soci-3.2.1.tar.gz

.PHONY: SOCI
SOCI: $(BUILD)/libsoci_core.so.3.2.1

$(BUILD)/libsoci_core.so.3.2.1: $(TARGET_BASE)/$(SOCI_PACKAGE_NAME)
	rm -rf $(BUILD)/SOCI
	mkdir -p $(BUILD)/SOCI
	cp $(TARGET_BASE)/$(SOCI_PACKAGE_NAME) $(BUILD)/SOCI
	cd $(BUILD)/SOCI && tar xf $(SOCI_PACKAGE_NAME) && \
		rm $(SOCI_PACKAGE_NAME) && \
		mv soci*/* . && \
		cmake . && \
		make all
	cp -v $(BUILD)/SOCI/lib/* $(BUILD)

