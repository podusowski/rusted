CC=g++
CFLAGS+=-Wall

DEPS := $(addprefix $(BUILD)/$(BUILDPREFIX)/, $(SOURCES:.cpp=.d))

MAKEDEP=$(CC) -MM $(CFLAGS) $< | \
		sed "s!`basename $*\\\.o`!$*.o!" | \
		sed "s!$*\.o!$(BUILD)/$(BUILDPREFIX)/$*.o!" \
		> $(BUILD)/$(BUILDPREFIX)/$*.d

-include $(DEPS)

%.hpp: ;

$(BUILD)/$(BUILDPREFIX)/%.o: %.cpp
	@mkdir -p `dirname $@`
	@$(MAKEDEP)
	@/bin/echo -e "$(FONT_BOLD)$(CC)$(FONT_RESET) $<" 
	@$(CC) $(CFLAGS) -c $< -o $@