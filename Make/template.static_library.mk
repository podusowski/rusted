AR:=ar rcs

#$(1)_OBJS:=$(addprefix $(BUILD)/,$($(1)_SOURCES:.cpp=.o))
define static_library_TEMPLATE

$(1)_OBJS:=$($(1)_SOURCES:.cpp=.o)

$$(info $($(1)_OBJS))

$(1): $$($(1)_OBJS)
	mkdir -p $(BUILD)/`basename $(1)`
	@/bin/echo -e "$(FONT_BOLD)$(AR) `basename $(1)` $(FONT_RESET)($$($(1)_OBJS))"
	@echo ""
	@$(AR) $(BUILD)/$$@ $($(1)_OBJS)

endef

