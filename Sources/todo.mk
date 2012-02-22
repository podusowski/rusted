TARGET:=todo
TYPE:=custom

.PHONY: todo
todo:
	@/bin/echo -e '$(FONT_BOLD)todo$(FONT_RESET)'
	@grep -iR TODO $(TARGET_BASE) | grep -v todo.mk
	
