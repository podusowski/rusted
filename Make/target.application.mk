# if appname names like one of the file, default make rule will be invoked
#$(APPNAME): ;
#.PHONY: $(APPNAME)
$(APPNAME): $(RESOURCES) $(DEPENDENCIES) $(OBJS)	
	@mkdir -p $(BUILD)
	@/bin/echo -e "$(FONT_BOLD)link `basename $@`$(FONT_RESET) ($(LDFLAGS))"
	@echo ""
	@$(CC) $(OBJS) $(LDFLAGS) -o $(BUILD)/$@

