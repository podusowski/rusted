TARGET:=Contents
TYPE:=custom

.PHONY: Contents 
Contents:
	@mkdir -p $(BUILD)
	@rm $(BUILD)/Contents -rf
	cp -rl $(TARGET_BASE) $(BUILD)/Contents

