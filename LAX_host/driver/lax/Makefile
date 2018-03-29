#
# Copyright 2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
export DEV_ROOT_DIR = $(PWD)/../oal
SUBDIRS = build-linux-kernel



$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(SUBDIRS) all clean

clean: $(SUBDIRS)
all: $(SUBDIRS)
