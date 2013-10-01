define static_library_TEMPLATE

$(1): $$($(1)_OBJS)
	@mkdir -p $$(BUILD)
	@/bin/echo -e "$(FONT_BOLD)$(AR)$(FONT_RESET) `basename $$@` "
	@$(AR) $$(BUILD)/$$@ $($(1)_OBJS)

endef

