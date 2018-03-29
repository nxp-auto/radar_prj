1) clone repo:
https://bitbucket.sw.nxp.com/projects/ALB/repos/linux/browse
and checkout from the specific release tag

2) Copy LAX specific files (including landshark):
	- from/to include/uapi/linux  
	- from/to drivers/misc/vspa  
3) Use changes for vspa and landshark in Kconfig and Makefile
4)to build vmlinux, Image and *dtb:
make ARCH=arm64 CROSS_COMPILE=/work/gcc-linaro-4.9-2015..../bin/aarch64-linux-gnu- s32xxxx_defconfig
make ARCH=arm64 CROSS_COMPILE=/work/gcc-linaro-4.9-2015..../bin/aarch64-linux-gnu-

And for LAX, add to the make options above:
KCPPFLAGS+=LAX_PLATFORM=PLATFORM_RRM


5) Follow instructions at 
https://bitbucket.sw.nxp.com/projects/ALB/repos/alb-tools/browse/vdk?at=refs%2Ftags%2Fbsp14.0
to create a VDK vpconfig.
