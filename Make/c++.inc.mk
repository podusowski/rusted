CC=g++
CFLAGS+=-Wall -pedantic

$(BUILD)/$(BUILDPREFIX)/%.o: %.cpp
	@mkdir -p `dirname $@`
	@$(MAKEDEP)
	@/bin/echo -e "\033[34mCPP \033[0m$<" 
	@$(CC) $(CFLAGS) -c $< -o $@
