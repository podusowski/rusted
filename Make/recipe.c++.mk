CC=g++
CFLAGS+=-Wall -O0

define recipe_c++_TEMPLATE

$(1)/%.o: $(2)/%.cpp
	@mkdir -p `dirname $$@`
	@echo "$(FONT_BOLD)$(CC)$(FONT_RESET) $$(subst $(PWD)/,,$$<) "
	@$$(CC) $$(CFLAGS) -c $$< -o $$@
	@$$(CC) -MM $$(CFLAGS) $$< | sed 's!$$(notdir $$@):!$$@:!' > $$@.d

endef
