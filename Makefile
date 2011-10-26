BUILD=$(PWD)/_build
SOURCE_DIR=$(PWD)/Sources

CPUS := `grep processor /proc/cpuinfo | wc -l`

MFLAGS := --no-builtin-rules
MFLAGS += --no-print-directory
MFLAGS += --include-dir $(PWD)/Make

ifeq ($(MAKELEVEL), 0)
  MFLAGS += -j$(CPUS)
endif

MFLAGS += -s

.DEFAULT:
	@$(MAKE) -f $@.mk \
		--directory `dirname \`find $(SOURCE_DIR) -name $@.mk\`` \
		$(MFLAGS) \
		BUILD=$(BUILD) \
		TOP_DIR=$(PWD) \
		$@ || ( echo "failed on $@"; exit 1 )

.PHONY: help
help:
	@echo "targets:"
	@echo
	@find $(SOURCE_DIR) -name *.mk -exec basename {} \; | sed s/.mk//

.PHONY: all
all:
	@find $(SOURCE_DIR) -name *.mk -exec basename {} \; | sed s/.mk// | xargs make

.PHONY: clean
clean:
	rm -rf $(BUILD)
