CC=g++
CFLAGS+=-Wall

$(BUILD)/$(BUILDPREFIX)/%.o: %.cpp
	@mkdir -p `dirname $@`
	@$(MAKEDEP)
	@/bin/echo -e "\033[34m$(CC) \033[0m$<" 
	@$(CC) $(CFLAGS) -c $< -o $@
