DEPS := $(addprefix $(BUILD)/$(BUILDPREFIX)/, $(SOURCES:.cpp=.d))

MAKEDEP=$(CC) -MM $(CFLAGS) $< | \
		sed "s!`basename $*\\\.o`!$*.o!" | \
		sed "s!$*\.o!$(BUILD)/$(BUILDPREFIX)/$*.o!" \
		> $(BUILD)/$(BUILDPREFIX)/$*.d

-include $(DEPS)

%.hpp: ;
