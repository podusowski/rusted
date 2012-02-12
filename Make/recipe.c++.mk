CC=g++
CFLAGS+=-Wall -O0

#$(BUILD)/%.o: %.cpp
#	@mkdir -p `dirname $@`
#	@/bin/echo -ne "$(FONT_BOLD)$(CC)$(FONT_RESET) $< "
#	@$(MAKEDEP)
#	@/usr/bin/time -f %E -- $(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@echo compile c++: $@ from $< $(CFLAGS)

