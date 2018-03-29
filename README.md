## Build steps for lax driver

0. Set LAX folder variable
    $ export LAX_DIR= <path to lax module folder>

1. Set cross-compilation environment
	```bash
	# Linux Kernel folder (ssh://gitolite3@git.codeaurora.org:22/external/private_la1575/ua-linux)
    $ export KERNEL_DIR=<absolute path to kernel directory ua-linux>

	# Cross Compiler for ARM
	$ export TOOLCHAIN=<absolute path to linaro gcc folder>
	$ export PATH=$TOOLCHAIN/bin:$PATH
	$ export CROSS_COMPILE=$TOOLCHAIN/bin/aarch64-linux-gnu-


2. Build rsdk_lax_driver.ko
	```bash
    $ cd  $LAX_DIR/LAX_host/driver/lax
	# Build lax kernel module
    $  make 

	```
