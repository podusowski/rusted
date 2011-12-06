AR=ar rcs

# if appname names like one of the file, default make rule will be invoked
$(LIBNAME): ;
$(LIBNAME): $(BUILD)/$(LIBNAME)
$(BUILD)/$(LIBNAME): $(OBJS)
	@/bin/echo -e "\033[1;32m$(AR) \033[00m`basename $@` ($(OBJS))"
	@echo ""
	@$(AR) $@ $(OBJS)
