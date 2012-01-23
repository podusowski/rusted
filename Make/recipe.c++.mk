CC=g++
CFLAGS+=-Wall -O0

DEPS := $(addprefix $(BUILD)/$(BUILDPREFIX)/, $(SOURCES:.cpp=.d))

MAKEDEP=$(CC) -MM $(CFLAGS) $< | \
		sed "s!`basename $*\\\.o`!$*.o!" | \
		sed "s!$*\.o!$(BUILD)/$(BUILDPREFIX)/$*.o!" \
		> $(BUILD)/$(BUILDPREFIX)/$*.d

		#&& mv $(BUILD)/$(BUILDPREFIX)/$*.d.candidate $(BUILD)/$(BUILDPREFIX)/$*.d

-include $(DEPS)

%.hpp: ;

$(BUILD)/$(BUILDPREFIX)/%.o: %.cpp
	@mkdir -p `dirname $@`
	@/bin/echo -ne "$(FONT_BOLD)$(CC)$(FONT_RESET) $< "
	@$(MAKEDEP)
	@/usr/bin/time -f %E -- $(CC) $(CFLAGS) -c $< -o $@
