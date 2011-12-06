.PHONY: Contents 
Contents:
	@echo "COPY Contents"
	@mkdir -p $(BUILD)
	@rm $(BUILD)/Contents -rf
	@cp -rl . $(BUILD)/Contents

