define application_TEMPLATE

$(1): $$(DEPENDENCIES) $$($(1)_OBJS)
	@mkdir -p $$(BUILD)
	@/bin/echo -e "$(FONT_BOLD)link `basename $(1)` $(FONT_RESET)"
	@$$(CC) $$($(1)_OBJS) $$(LDFLAGS) -o $$(BUILD)/$$@

endef

