##
# Copyright 2018 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

.DEFAULT_GOAL := all

ifdef NATIVE_BUILD

ifneq (,$(findstring native,$(MAKECMDGOALS)))
CROSS_COMPILE :=
endif

native: all

else
native:
endif
