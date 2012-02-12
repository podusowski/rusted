define recipe_resources_TEMPLATE

$$(TARGET): $$(BUILD)/$(1)
$$(BUILD)/$(1): $$(TARGET_BASE)/$(1)
	@mkdir -p $$(BUILD)
	@/bin/echo -e '$$(FONT_BOLD)cp$$(FONT_RESET) $(1)'
	@cp -rt $$(BUILD)/ $$(TARGET_BASE)/$(1)

endef
