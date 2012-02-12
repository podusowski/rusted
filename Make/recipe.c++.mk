CC=g++
CFLAGS+=-Wall -O0

#$(BUILD)/%.o: %.cpp
#	@mkdir -p `dirname $@`
#	@/bin/echo -ne "$(FONT_BOLD)$(CC)$(FONT_RESET) $< "
#	@$(MAKEDEP)
#	@/usr/bin/time -f %E -- $(CC) $(CFLAGS) -c $< -o $@

define recipe_c++_TEMPLATE

$(1)/%.o: $(2)/%.cpp
	@mkdir -p `dirname $$@`
	@echo "$(FONT_BOLD)$(CC)$(FONT_RESET) $$< "
	@$$(CC) $$(CFLAGS) -c $$< -o $$@

endef
