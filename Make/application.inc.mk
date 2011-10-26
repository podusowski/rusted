# if appname names like one of the file, default make rule will be invoked
$(APPNAME): ;
$(APPNAME): $(BUILD)/$(APPNAME) 
$(BUILD)/$(APPNAME): $(DEPENDENCIES) $(OBJS)	
	@mkdir -p $(BUILD)
	@/bin/echo -e "\033[1;32mLINK \033[0m`basename $@` ($(LDFLAGS))"
	@echo ""
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

