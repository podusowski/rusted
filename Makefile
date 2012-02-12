BUILD=$(PWD)/_build
SOURCE_DIR=$(PWD)/Sources

include Make/util.colors.mk
include Make/template.static_library.mk
include Make/template.application.mk
include Make/recipe.c++.mk

define include_target_TEMPLATE

TARGET_BASE=$(shell dirname $(1))

TARGET:=
TYPE:=
SOURCES:=
CFLAGS:=
DEPENDENCIES:=

include $(1)

ifeq ($$(TARGET),)
$$(error no target specified in $(1))
endif

ifeq ($$(TYPE),)
$$(error no type specified in $(1))
endif

OBJS:=$$(addprefix $$(BUILD)/$$(TARGET)/,$$(SOURCES))
OBJS:=$$(OBJS:.cpp=.o)
$$(TARGET)_OBJS:=$$(OBJS)

SOURCES:=$$(addprefix $$(TARGET_BASE)/,$$(SOURCES))
$$(TARGET)_SOURCES:=$$(SOURCES)

#$$(info sources: $$($$(TARGET)_SOURCES))
#$$(info objs: $$($$(TARGET)_OBJS))

$$(TARGET): CFLAGS:=$$(CFLAGS)
$$(TARGET): $$(DEPENDENCIES)

$$(eval $$(call recipe_c++_TEMPLATE,$$(BUILD)/$$(TARGET),$$(TARGET_BASE)))
$$(eval $$(call $$(TYPE)_TEMPLATE,$$(TARGET)))

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

$(BUILD):
	mkdir -p $(BUILD)
