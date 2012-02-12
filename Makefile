CPUS:=$(shell cat /proc/cpuinfo | grep processor | wc -l)
JOBS:=$(shell echo $(CPUS)+2 | bc)

$(info $(CPUS) cpus detected, $(JOBS) processes will be used)

.PHONY: default
default:
	@make --no-print-directory -j$(JOBS) -f Make/Makefile JOBS=$(JOBS)

.DEFAULT:
	@make --no-print-directory -j$(JOBS) -f Make/Makefile $@ JOBS=$(JOBS)
