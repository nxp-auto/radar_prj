##
# Copyright 2017-2020 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

override CDEFS += -Wp,-MD,$@.d -Wp,-MT,$@

OBJS_DEP  = $(addsuffix .d,$(OBJS))

$(OBJS_DEP): | $(OBJDIR)

all: $(OBJS_DEP)

-include  $(wildcard $(OBJDIR)/*.o.d)

