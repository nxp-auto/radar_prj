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

ifeq ($(OS),zephyr)
INCFLAGS2 := -I$(TEST_ROOT_DIR)/scripts/uml-generator/cgstubs/$(OS) $(foreach tmp_var,$(INCDIR),-I$(tmp_var)) \
				-I$(DRIVER_BUILD_DIR)/libs/kernel/test_common/build-zephyr-kernel/build/zephyr/include/generated \
				-I$(ZEPHYR_BASE)/kernel/include -I$(ZEPHYR_BASE)/arch/arm64/include -I$(ZEPHYR_BASE)/include \
				-I$(DRIVER_BUILD_DIR)/build/zephyr/include/generated -I$(ZEPHYR_BASE)/lib/libc/newlib/include \
				-I$(TEST_ROOT_DIR)/tests/$(MODULE_NAME)/include -I$(TEST_ROOT_DIR)/include \
				-I$(TEST_ROOT_DIR)/include/zephyr -I$(TEST_ROOT_DIR)/oal/include -I$(TEST_ROOT_DIR)/oal/include/zephyr \
				-I$(TEST_ROOT_DIR)/oal/libs/kernel/common/include -I$(ZEPHYR_BASE)/include/posix -I$(GNUARMNXP_TOOLCHAIN_PATH)/include
else
INCFLAGS2 := -I$(TEST_ROOT_DIR)/scripts/uml-generator/cgstubs/$(OS) $(foreach tmp_var,$(INCDIR),-I$(tmp_var))
endif

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


ifneq ($(OS),zephyr)
override CDEFS += "-DPAGE_SIZE=0x1000"
endif

ifeq ($(OS),zephyr)
CC = $(GNUARMNXP_TOOLCHAIN_PATH)/bin/aarch64-none-elf-gcc
NM = $(GNUARMNXP_TOOLCHAIN_PATH)/bin/aarch64-none-elf-nm
%.expand %.o: %.c
	@echo "	[GCC] $@ <- $^"
	$(info CUR_DIR = $(PWD))
	@$(CC) -w -g3 -O0 -fdump-rtl-expand=$@ -fno-leading-underscore -U$(OS) -DOS=$(OS) $(INCFLAGS2) $(CDEFS) \
		-imacros $(DRIVER_BUILD_DIR)/build/zephyr/include/generated/autoconf.h \
		-fmacro-prefix-map=$(ZEPHYR_BASE)=ZEPHYR_BASE -fmacro-prefix-map=$(ZEPHYR_BASE)/../=WEST_TOPDIR \
		-std=c99 -c $^
else
%.expand %.o: %.c
	@echo "	[GCC] $@ <- $^"
	@$(CC) -w -g3 -O0 -fdump-rtl-expand=$@ -fno-leading-underscore -U$(OS) -DOS=$(OS) $(INCFLAGS2) $(CDEFS) -c $^
endif

%.funcs: %.o
	@echo "	[NM]  $@ <- $^"
	@$(NM) --line-number --defined-only $^ | grep -e "^[0-9a-f]\+ [tTwW] [^\.$$]" | awk '{print $$3"#"$$4}' > $@

%.callgraph: %.expand
	@echo "	[EGT] $@ <- $^"
	@$(EGYPT) --include-external $^ > $@


clean clean_graph:
	@rm -f *.callgraph *.expand *.funcs


