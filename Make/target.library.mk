AR=ar rcs

# if appname names like one of the file, default make rule will be invoked
#$(LIBNAME): ;
#.PHONY: $(LIBNAME)
$(LIBNAME): $(DEPENDENCIES) $(OBJS)
	@/bin/echo -e "$(FONT_BOLD)$(AR) `basename $@` $(FONT_RESET)($(OBJS))"
	@echo ""
	@$(AR) $(BUILD)/$@ $(OBJS)
