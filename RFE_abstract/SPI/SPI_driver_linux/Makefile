#
# Copyright 2019-2023 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

_RSDK_ROOTPATH := ../../..
	
RSDK_ABS_ROOTPATH := $(PWD)/$(_RSDK_ROOTPATH)


ifeq ($(DBG_PRINT),true)
	PRINTK_EN := -DPRINTK_ENABLE
endif


SRCS := spi_driver_module.c 		
INCLUDES := $(RSDK_ABS_ROOTPATH)/api/

OBJS := $(SRCS:.c=.o) 


#----- kbuild config: ----------------------------
MODULE_NAME := rsdk_spi_driver
ARCH ?= arm64
EXTRA_CFLAGS := -Wall -g $(addprefix -I,$(INCLUDES))
CFLGS := -std=gnu99 -Ulinux -Dlinux -DOS=linux -DS32R45 $(PRINTK_EN)

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-objs := $(OBJS)

.PHONY: all cleanall module clean_module modules_install clean

all: module

cleanall: clean_module

module:
	$(MAKE) -C $(KERNEL_DIR) KCPPFLAGS="$(CFLGS)" ARCH=$(ARCH) M=$(CURDIR) modules -j1


modules_install:
	$ make -C $(KERNEL_DIR) M="$(CURDIR)" modules_install

clean_module clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) clean
	rm -f *.o *.*.cmd *.order *.mod
	
print-%:
	@echo $* = $($*)

