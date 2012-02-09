BUILD=$(PWD)/_build
SOURCE_DIR=$(PWD)/Sources

TARGET_CONFIGS := $(shell find $(SOURCE_DIR) -name *.mk2)
include $(TARGET_CONFIGS)

include Make/util.colors.mk
include Make/template.static_library.mk
include Make/recipe.c++.mk

$(foreach i, $(STATIC_LIBRARIES), $(eval $(call static_library_TEMPLATE,$(i))))
$(foreach i, $(STATIC_LIBRARIES), $(info $(call static_library_TEMPLATE,$(i))))

.PHONY: help
help:
	@echo Static libraries: $(STATIC_LIBRARIES)

.PHONY: clean
clean:
	rm -rf $(BUILD)
