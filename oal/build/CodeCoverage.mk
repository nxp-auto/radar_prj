##
# Copyright 2018-2019 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

ifeq ($(FORCE_NO_CODE_COVERAGE),)
ifeq ($(ENABLE_CODE_COVERAGE),1)
CFLAGS      += -ftest-coverage -fprofile-arcs
LDFLAGS     += -lgcov $(_INJECT_LIBRARY)
endif
endif
