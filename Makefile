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
	@( test `find $(SOURCE_DIR) -name $@.mk | wc -l` -eq 1 || ( echo "there two $@.mk files" ; exit 1 ) ) && \
	$(MAKE) -f $@.mk \
		--directory `dirname \`find $(SOURCE_DIR) -name $@.mk\`` \
		$(MFLAGS) \
		BUILD=$(BUILD) \
		TOP_DIR=$(PWD) \
		$@ \
	|| ( echo "failed to build $@"; exit 1 )

.PHONY: help
help:
	@echo "Available targets:"
	@echo
	@find $(SOURCE_DIR) -name *.mk -exec basename {} \; | sed s/.mk//

.PHONY: all
all:
	@find $(SOURCE_DIR) -name *.mk -exec basename {} \; | sed s/.mk// | xargs make

.PHONY: clean
clean:
	rm -rf $(BUILD)
