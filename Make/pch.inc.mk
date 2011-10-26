ifneq ($(PCH),)
$(OBJS): $(PCH).gch
CFLAGS_PCH=-include $(PCH) -Winvalid-pch
PCH_DEPS := $(addprefix $(BUILD)/$(BUILDPREFIX)/, $(PCH).d))
-include $(PCH_DEPS)
endif

%.hpp.gch: %.hpp
	@echo "\033[34mGCH \033[0m$< > $@" 
	@$(MAKEDEP_PCH)
	@$(CC) $(CFLAGS) -x c++-header $<
