define application_TEMPLATE

$(1): $$(BUILD)/$(1)
$$(BUILD)/$(1): $(BUILD) $$($(1)_OBJS)
	@/bin/echo -e "$(FONT_BOLD)link `basename $(1)` $(FONT_RESET)($$($(1)_OBJS))"

endef

