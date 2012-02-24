CPUS:=$(shell cat /proc/cpuinfo | grep processor | wc -l)
JOBS:=$(shell echo $(CPUS)+1 | bc)

$(info $(CPUS) cpus detected, $(JOBS) processes will be used)

.PHONY: default
default:
	@$(MAKE) -r --no-print-directory -j$(JOBS) -f Make/Makefile JOBS=$(JOBS)

.DEFAULT:
	@make -r --no-print-directory -j$(JOBS) -f Make/Makefile $@ JOBS=$(JOBS)
