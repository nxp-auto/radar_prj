##
# Copyright 2018-2019 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

## Compile device tree files and generate a hexdump of the compiled device tree
#  blob, so that it can be referred directly in sources.
##

ifdef DTS_FILES

.PHONY: clean_dtb

DTC ?= dtc

ifneq (,$(KERNEL_DIR))
    DTC = $(KERNEL_DIR)/scripts/dtc/dtc
endif

VPATH += $(DRIVER_BUILD_DIR)$(OBJDIR)
DTS_SRCS = $(addprefix $(OBJDIR)/,$(DTS_FILES:.dts=.c))
SRCS += $(notdir $(DTS_SRCS))

.PRECIOUS: $(DTS_SRCS)

vpath %.dts   $(VPATH)

DTB_FILES = $(addprefix $(OBJDIR)/,$(DTS_FILES:.dts=.dtb))

HEXDUMP ?= xxd

all: $(DTB_FILES) $(DTS_SRCS)
prebuild: $(DTB_FILES) $(DTS_SRCS)

$(OBJDIR)/%.dtb: %.dts | $(OBJDIR)
	@echo "	[DTC] $^ <= $@"
	@$(DTC) -q -O dtb -o $@ $^

$(OBJDIR)/%.c: $(OBJDIR)/%.dtb
	@echo "	[XXD] $^ <= $@"
	$(HEXDUMP) -g4 -u -i $^ $@
	sed -ie "s#[[:alnum:]_]\+\[\]#$(basename $(notdir $@))_dtb[]#g" $@

clean: clean_dtb

clean_dtb:
	@rm -f $(DTB_FILES) $(DTS_SRCS)

endif
