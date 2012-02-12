define application_TEMPLATE

$(1): $$(BUILD)/$(1)
$$(BUILD)/$(1): $$(DEPENDENCIES) $(BUILD) $$($(1)_OBJS)
	@/bin/echo -e "$(FONT_BOLD)link `basename $(1)` $(FONT_RESET)"
	@$$(CC) $$($(1)_OBJS) $$(LDFLAGS) -o $$@

endef

