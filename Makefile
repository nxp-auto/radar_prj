#
# Copyright 2021-2023 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

SRC := $(shell pwd)/

MODULE_SRC := $(SRC)/ICC_module
SAMPLE_MODULE_SRC := $(SRC)/ICC_Sample_module

.PHONY: build modules_install clean

all: build

build:
	$(MAKE) -C $(SRC)/oal/libs/kernel/linux-write/build-linux-kernel/ OS=linux

modules_install:
	echo	"Nothing to do to install oal_libs"

headers_install:
	$(MAKE) -C $(MODULE_SRC) headers_install

clean:
	$(MAKE) -C $(SRC)/oal/libs/kernel/linux-write/build-linux-kernel/ clean
