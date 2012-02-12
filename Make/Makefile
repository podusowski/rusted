BUILD=$(PWD)/_build
SOURCE_DIR=$(PWD)/Sources

include Make/util.colors.mk
include Make/template.static_library.mk
include Make/template.application.mk
include Make/recipe.c++.mk
include Make/recipe.resources.mk

.PHONY: help
help:
	@echo -e '$(shell basename $(PWD)) targets:\n$(TARGETS_HELP)'

.PHONY: clean
clean:
	@/bin/echo -e '$(FONT_BOLD)rm$(FONT_RESET) $(BUILD)'
	@rm -rf $(BUILD)

$(BUILD):
	@/bin/echo -e '$(FONT_BOLD)mkdir$(FONT_RESET) $(BUILD)'
	@mkdir -p $(BUILD)

define include_target_TEMPLATE

TARGET_BASE=$(shell dirname $(1))

TARGET:=
TYPE:=
SOURCES:=
CFLAGS:=
LDFLAGS:=
DEPENDENCIES:=
RESOURCES:=

include $(1)

ifeq ($$(TARGET),)
$$(error no target specified in $(1))
endif

ifeq ($$(TYPE),)
$$(error no type specified in $(1))
endif

TARGETS+=$$(TARGET)
TARGETS_HELP+=\t$$(FONT_BOLD)$$(TARGET)$$(FONT_RESET) ($$(TYPE))\n

OBJS:=$$(addprefix $$(BUILD)/build.$$(TARGET)/,$$(SOURCES))
OBJS:=$$(OBJS:.cpp=.o)
$$(TARGET)_OBJS:=$$(OBJS)

DEPS:=$$(addprefix $$(BUILD)/build.$$(TARGET)/,$$(SOURCES))
DEPS:=$$(DEPS:.cpp=.o.d)
-include $$(DEPS)

SOURCES:=$$(addprefix $$(TARGET_BASE)/,$$(SOURCES))
$$(TARGET)_SOURCES:=$$(SOURCES)
$$(SOURCES): $$(DEPENDENCIES)

$$(TARGET): CFLAGS:=$$(CFLAGS)
$$(TARGET): LDFLAGS:=$$(LDFLAGS)
$$(TARGET): DEPENDENCIES:=$$(DEPENDENCIES)
$$(TARGET): TARGET_BASE:=$$(TARGET_BASE)
$$(TARGET): $$(DEPENDENCIES)

ifneq ($$(RESOURCES),)
$$(eval $$(call recipe_resources_TEMPLATE,$$(RESOURCES)))
endif

$$(eval $$(call recipe_c++_TEMPLATE,$$(BUILD)/build.$$(TARGET),$$(TARGET_BASE)))
$$(eval $$(call $$(TYPE)_TEMPLATE,$$(TARGET)))

endef

#find and include all target configs
TARGETS:=
TARGETS_HELP:=
TARGET_CONFIGS := $(shell find $(SOURCE_DIR) -name *.mk)
$(foreach i, $(TARGET_CONFIGS),$(eval $(call include_target_TEMPLATE,$(i))))

.PHONY: all
all: $(TARGETS)

