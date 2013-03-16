PROFILE:=default

include $(PROFILE).buildprofile
$(info profile: $(PROFILE))
$(info cflags: $(CFLAGS))

BUILD_BASE:=$(PWD)/_build
BUILD:=$(BUILD_BASE)/$(PROFILE)/
SOURCE_DIR=$(PWD)/Sources

VPATH:=$(BUILD)

include Make/util.colors.mk
include Make/template.static_library.mk
include Make/template.application.mk
include Make/recipe.c++.mk
include Make/recipe.resources.mk

.PHONY: help
help:
	@echo -e '$(shell basename $(PWD)) targets:\n$(FONT_BOLD)\tall$(FONT_RESET)\n$(TARGETS_HELP)'

.PHONY: clean
clean:
	@/bin/echo -e '$(FONT_BOLD)rm$(FONT_RESET) $(BUILD)'
	@rm -rf $(BUILD_BASE)

$(BUILD):
	@/bin/echo -e '$(FONT_BOLD)mkdir$(FONT_RESET) $(BUILD)'
	@mkdir -p $(BUILD)

# from profile
DEFAULT_CFLAGS:=$(CFLAGS)

#
# this template is executed for each mk file found in the project
# tree consuming its targets
#
define include_target_TEMPLATE

  #
  # root directory of the target
  #
  TARGET_BASE=$(shell dirname $(1))

  #
  # reset variables before including mk file
  #
  TARGET:=
  TYPE:=
  SOURCES:=
  CFLAGS:=$$(DEFAULT_CFLAGS)
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

  #
  # fill the help string
  #
  ifneq ($$(DEPENDENCIES),)
    HELP_DEPENDS_ON:=depends on$$(DEPENDENCIES)
  else
    HELP_DEPENDS_ON:=
  endif
  TARGETS_HELP+=\t$$(FONT_BOLD)$$(TARGET)$$(FONT_RESET) ($$(TYPE)) $$(HELP_DEPENDS_ON)\n

  OBJS:=$$(addprefix $$(BUILD)/build.$$(TARGET)/,$$(SOURCES))
  OBJS:=$$(OBJS:.cpp=.o)
  $$(TARGET)_OBJS:=$$(OBJS)

  DEPS:=$$(addprefix $$(BUILD)/build.$$(TARGET)/,$$(SOURCES))
  DEPS:=$$(DEPS:.cpp=.o.d)
  -include $$(DEPS)

  SOURCES:=$$(addprefix $$(TARGET_BASE)/,$$(SOURCES))
  $$(TARGET)_SOURCES:=$$(SOURCES)
  $$(SOURCES): $$(DEPENDENCIES)
  $$(OBJS): $(1)

  $$(TARGET): CFLAGS:=$$(CFLAGS)
  $$(TARGET): LDFLAGS:=$$(LDFLAGS)
  $$(TARGET): DEPENDENCIES:=$$(DEPENDENCIES)
  $$(TARGET): TARGET_BASE:=$$(TARGET_BASE)
  $$(TARGET): $$(DEPENDENCIES)

  ifneq ($$(RESOURCES),)
    $$(foreach i,$$(RESOURCES),$$(eval $$(call recipe_resources_TEMPLATE,$$(i))))
  endif

  #
  # include c++ building rules
  #
  $$(eval $$(call recipe_c++_TEMPLATE,$$(BUILD)/build.$$(TARGET),$$(TARGET_BASE)))

  #
  # include final target rule (eg linking the app)
  #
  $$(eval $$(call $$(TYPE)_TEMPLATE,$$(TARGET)))

endef

#
# find and include all target configs
#
TARGETS:=
TARGETS_HELP:=
TARGET_CONFIGS := $(shell find $(SOURCE_DIR) -name *.mk)
$(foreach i, $(TARGET_CONFIGS),$(eval $(call include_target_TEMPLATE,$(i))))

.PHONY: all
all: $(TARGETS)

