define recipe_resources_TEMPLATE

RESOURCES_DESTINATION:=$$(addprefix $$(BUILD)/,$(1))
RESOURCES_SOURCE:=$$(addprefix $$(TARGET_BASE)/,$(1))

$$(TARGET): $$(RESOURCES_DESTINATION)
$$(RESOURCES_DESTINATION): $$(RESOURCES_SOURCE)
	@mkdir -p $$(BUILD)
	@/bin/echo -e '$$(FONT_BOLD)cp$$(FONT_RESET) $(1)'
	@cp -rt $$(BUILD)/ $$(RESOURCES_SOURCE)

endef
