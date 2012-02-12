AR:=ar rcs

define static_library_TEMPLATE

$(1): $$(BUILD)/$(1).a
$$(BUILD)/$(1).a: $$($(1)_OBJS)
	mkdir -p $(BUILD)/`basename $(1)`
	@/bin/echo -e "$(FONT_BOLD)$(AR) $$@ $(FONT_RESET)($$($(1)_OBJS))"
	@echo ""
	@$(AR) $$@ $($(1)_OBJS)

endef

