##
# Copyright 2017-2020, 2022 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##


CC ?= gcc
NM ?= nm

TEST_ROOT_DIR ?= $(DEV_ROOT_DIR)/../
# https://www.gson.org/egypt/
EGYPT ?= $(TEST_ROOT_DIR)/scripts/egypt.sh

EXPANDS = $(SRCS:.c=.expand)
CALL_GRAPHS = $(SRCS:.c=.callgraph)
FUNCS = $(SRCS:.c=.funcs)
PREEXPANDS = $(SRCS:.c=.pregraph)

GRAPH = $(MODULE_NAME)$(STATIC_LIB_NAME).graph

vpath %.c   $(VPATH)
vpath %.S   $(VPATH)
vpath %.cpp $(VPATH)

INCFLAGS2 := -I$(TEST_ROOT_DIR)/scripts/uml-generator/cgstubs/$(OS) $(foreach tmp_var,$(INCDIR),-I$(tmp_var))

graph: $(CALL_GRAPHS) $(FUNCS)

ifeq ($(OS)$(OS_WORLD),linuxkernel)
    override CDEFS += "-D__KERNEL__"
endif

ifeq ($(OS),linux)
    override CDEFS += -nostdinc -DOAL_LOG_SUPPRESS_ERROR -DOAL_LOG_SUPPRESS_WARNING -DOAL_LOG_SUPPRESS_DEBUG -DOAL_LOG_SUPPRESS_NOTE
endif

ifeq ($(OS),sa)
    ARCH ?= arm64
    CC = $(CROSS_COMPILE)gcc
    NM = $(CROSS_COMPILE)nm
    override CDEFS += -DNEED_STDINT_DEFS
endif

override CDEFS += "-DPAGE_SIZE=0x1000"

%.expand %.o: %.c
	@echo "	[GCC] $@ <- $^"
	@$(CC) -w -g3 -O0 -fdump-rtl-expand=$@ -fno-leading-underscore -U$(OS) -DOS=$(OS) $(INCFLAGS2) $(CDEFS) -c $^

%.funcs: %.o
	@echo "	[NM]  $@ <- $^"
	@$(NM) --line-number --defined-only $^ | grep -e "^[0-9a-f]\+ [tTwW] [^\.$$]" | awk '{print $$3"#"$$4}' > $@

%.callgraph: %.expand
	@echo "	[EGT] $@ <- $^"
	@$(EGYPT) --include-external $^ > $@


clean clean_graph:
	@rm -f *.callgraph *.expand *.funcs