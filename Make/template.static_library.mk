AR:=ar rcs

define static_library_TEMPLATE

$(1): $$(BUILD)/$(1)
$$(BUILD)/$(1): $$(BUILD) $$($(1)_OBJS)
	@/bin/echo -e "$(FONT_BOLD)$(AR)$(FONT_RESET) `basename $$@` "
	@$(AR) $$@ $($(1)_OBJS)

endef

