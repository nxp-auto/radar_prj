#
# Copyright 2018-2023 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
export DEV_ROOT_DIR = $(LAXTOPDIR)/../oal
SUBDIRS = build-$(OS)-kernel



$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(SUBDIRS) all clean cleantmp

clean: $(SUBDIRS)
cleantmp: $(SUBDIRS)
all: $(SUBDIRS)
