##
# Copyright 2017-2020 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

SUBDIRS_PATTERN := $(dir $(shell find -maxdepth 10 -name Makefile -type f))

EXISTING_SUBDIRS := $(foreach sdir,$(SUBDIRS_PATTERN),           \
                 $(if $(strip                                    \
                        $(wildcard $(sdir)/../BUILD.mk)          \
                        $(wildcard $(sdir)/../../BUILD.mk)       \
                       ),$(sdir),))

# Filter build-* directories
SUBDIRS := $(foreach sdir,$(EXISTING_SUBDIRS),                   \
                 $(if $(filter build-%,$(notdir $(patsubst %/,%,$(sdir)))) \
                   ,$(sdir),))

ifneq (,$(findstring linux,$(MAKECMDGOALS)))
    MATCH := linux
    FILTER := %linux-kernel/ %linux-user/
endif

ifneq (,$(findstring native,$(MAKECMDGOALS)))
    MATCH := native_X
    FILTER := %linux-user/
endif

ifneq (,$(findstring linux-kernel,$(MAKECMDGOALS)))
    MATCH := linux-kernel
    FILTER := %linux-kernel/
endif

ifneq (,$(findstring linux-user,$(MAKECMDGOALS)))
    MATCH := linux-user
    FILTER := %linux-user/
endif

ifneq (,$(findstring qnx,$(MAKECMDGOALS)))
    MATCH := qnx
    FILTER := %qnx-user/ %qnx-kernel/
endif

ifneq (,$(findstring ghs,$(MAKECMDGOALS)))
    MATCH := ghs
    FILTER := %ghs-user/ %ghs-kernel/
endif

ifneq (,$(findstring sa,$(MAKECMDGOALS)))
   MATCH := sa
   FILTER := %sa-kernel/
endif

ifneq (,$(findstring zephyr,$(MAKECMDGOALS)))
    MATCH := zephyr
    FILTER := %zephyr-kernel/ %zephyr-user/
endif

ifdef FILTER
    SUBDIRS := $(filter $(FILTER), $(SUBDIRS))
    LIBS_SUBDIRS := $(addsuffix _LIB,$(SUBDIRS))
    APPS_SUBDIRS := $(addsuffix _APP,$(SUBDIRS))
endif

ifeq ($(MATCH), $(MAKECMDGOALS))
    MAKECMDGOALS := all
else
    ifeq (,$(findstring graph,$(MAKECMDGOALS)))
        MAKECMDGOALS := $(subst _$(MATCH),,$(MAKECMDGOALS))
    endif
endif

.PHONY: $(SUBDIRS) $(LIBS_SUBDIRS) $(APPS_SUBDIRS) all clean linux qnx sa ghs zephyr install native graph clean_graph barrier

.NOTPARALLEL: barrier

compile: $(LIBS_SUBDIRS) barrier $(APPS_SUBDIRS)
clean: $(SUBDIRS)
all: compile
source_drop: $(SUBDIRS)
install: compile $(SUBDIRS)
native: compile
graph: $(SUBDIRS)
clean_graph:$(SUBDIRS)

linux: compile
linux-kernel: compile
linux-user: compile
qnx: compile
sa: compile
ghs: compile
zephyr: compile

clean_linux: clean
clean_qnx: clean
clean_ghs: clean
clean_sa: clean
clean_zephyr: clean
install_linux: compile $(SUBDIRS)
install_qnx: compile $(SUBDIRS)
install_sa: compile $(SUBDIRS)
install_zephyr: compile $(SUBDIRS)

barrier:

$(LIBS_SUBDIRS):
	$(MAKE) -C $(subst _LIB,, $@) lib

$(APPS_SUBDIRS):
	$(MAKE) -C $(subst _APP,, $@) app

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

