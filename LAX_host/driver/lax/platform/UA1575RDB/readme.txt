Clone ssh://git@bitbucket.sw.nxp.com/~nxa14932/ua-linux.git (develop branch)
Clone ssh://git@bitbucket.sw.nxp.com/radarsw/rsdk.git

$ bash
$ export KERNEL_DIR=<your_path_to_ua-linux>
$ cp <rsdk>/LAX/LAX_host/driver/vspa/platform/UA1575RDB/fsl-la1575a.dtsi $KERNEL_DIR/arch/arm64/boot/dts/freescale/
$ source <gcc_toolchain>/fsl-qoriq/2.0/environment-setup-aarch64-fsl-linux 
$ export LDFLAGS= 

$ cd $KERNEL_DIR
$ make distclean 
$ ./scripts/kconfig/merge_config.sh arch/arm64/configs/freescale.config arch/arm64/configs/defconfig 
$ make -j3 
$ mkimage -f kernel-la1575a-rdb.its kernel-la1575a-rdb.itb 

$ cd <rsdk>/LAX/LAX_host/driver/vspa
$ make





