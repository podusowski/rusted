BUILD=$(PWD)/_build
SOURCE_DIR=$(PWD)/Sources

include Make/util.colors.mk
include Make/template.static_library.mk
include Make/recipe.c++.mk

define include_target_TEMPLATE

$(info target config: $(1))

TARGET_BASE=$(shell dirname $(1))

TARGET:=
TYPE:=
SOURCES:=
CFLAGS:=

include $(1)

OBJS:=$$(addprefix $$(BUILD)/,$$(SOURCES))
OBJS:=$$(OBJS:.cpp=.o)
$$(TARGET)_OBJS:=$$(OBJS)

SOURCES:=$$(addprefix $$(TARGET_BASE)/,$$(SOURCES))
$$(TARGET)_SOURCES:=$$(SOURCES)

$$(info target: $$(TARGET), type: $$(TYPE))
$$(info sources: $$($$(TARGET)_SOURCES))
$$(info objs: $$($$(TARGET)_OBJS))

$$(TARGET): CFLAGS:=$$(CFLAGS)

$$(eval $$(call recipe_c++_TEMPLATE,$$(BUILD),$$(TARGET_BASE)))
$$(info $$(call recipe_c++_TEMPLATE,$$(BUILD),$$(TARGET_BASE)))
$$(eval $$(call static_library_TEMPLATE,$$(TARGET)))

TARGETS+=$$(TARGET)

endef

#find and include all target configs
TARGET_CONFIGS := $(shell find $(SOURCE_DIR) -name *.mk)
$(foreach i, $(TARGET_CONFIGS),$(eval $(call include_target_TEMPLATE,$(i))))

.PHONY: help
help:
	@echo Targets: $(TARGETS)

.PHONY: clean
clean:
	rm -rf $(BUILD)
