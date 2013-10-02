define application_TEMPLATE

$(1): $$(DEPENDENCIES) $$($(1)_OBJS)
	@mkdir -p $$(BUILD)
	@/bin/echo -e "$(FONT_BOLD)link `basename $(1)$(APPLICATION_SUFFIX)` $(FONT_RESET)"
	@$$(CC) $$($(1)_OBJS) $$(LDFLAGS) -o $$(BUILD)/$$@$(APPLICATION_SUFFIX)

endef

