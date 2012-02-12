BUILD=$(PWD)/_build
SOURCE_DIR=$(PWD)/Sources

define include_target_TEMPLATE

$(info target config: $(1))

TARGET_BASE=$(shell dirname $(1))

SOURCES:=

include $(1)

SOURCES:=$$(addprefix $$(TARGET_BASE)/,$$(SOURCES))
$$(TARGET)_SOURCES:=$$(SOURCES)

$$(info target: $$(TARGET), type: $$(TYPE))
$$(info sources: $$($$(TARGET)_SOURCES))
$$(info basedir: $$(TARGET_BASE))

endef

#find and include all target configs
TARGET_CONFIGS := $(shell find $(SOURCE_DIR) -name *.mk2)
$(foreach i, $(TARGET_CONFIGS),$(eval $(call include_target_TEMPLATE,$(i))))
#include $(TARGET_CONFIGS)

include Make/util.colors.mk
include Make/template.static_library.mk
include Make/recipe.c++.mk

#generate all target rules
$(foreach i, $(STATIC_LIBRARIES), $(eval $(call static_library_TEMPLATE,$(i))))
#$(foreach i, $(STATIC_LIBRARIES), $(info $(call static_library_TEMPLATE,$(i))))

.PHONY: help
help:
	@echo Static libraries: $(STATIC_LIBRARIES)

.PHONY: clean
clean:
	rm -rf $(BUILD)
