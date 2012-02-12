define application_TEMPLATE

ifeq ($(TYPE),application)

$(1)_OBJS:=$($(1)_SOURCES:.cpp=.o)

$(1): $$($(1)_OBJS)
	mkdir -p $(BUILD)/`basename $(1)`
	@/bin/echo -e "$(FONT_BOLD)application `basename $(1)` $(FONT_RESET)($$($(1)_OBJS))"

else
	$(info Target is not application)
endif

endef

