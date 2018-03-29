cmd_/work/fork/caf_lax_driver/LAX_host/driver/lax/build-linux-kernel/../os_agnostic/src//cbuffer.o := /work/gcc-linaro-4.9-2015.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc -Wp,-MD,/work/fork/caf_lax_driver/LAX_host/driver/lax/build-linux-kernel/../os_agnostic/src//.cbuffer.o.d  -nostdinc -isystem /work/gcc-linaro-4.9-2015.05-x86_64_aarch64-linux-gnu/bin/../lib/gcc/aarch64-linux-gnu/4.9.3/include -I./arch/arm64/include -Iarch/arm64/include/generated/uapi -Iarch/arm64/include/generated  -Iinclude -I./arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Ulinux -DOS=linux -I/work/fork/caf_lax_driver/LAX_host/driver/lax/include -I/include -I/work/fork/caf_lax_driver/LAX_host/driver/lax/../oal/include -I/work/fork/caf_lax_driver/LAX_host/driver/lax/os_agnostic/inc -I/work/fork/caf_lax_driver/LAX_host/driver/lax/platform/UA1575RDB/inc -I/work/fork/caf_lax_driver/LAX_host/driver/lax/linux -I/work/fork/caf_lax_driver/LAX_host/driver/lax/include/uapi/linux -I/work/fork/caf_lax_driver/LAX_host/driver/lax/../../../LAX_dispatcher/common -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -fno-delete-null-pointer-checks -O2 --param=allow-store-data-races=0 -Wframe-larger-than=2048 -fno-stack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -DCC_HAVE_ASM_GOTO  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(cbuffer)"  -D"KBUILD_MODNAME=KBUILD_STR(rsdk_lax_driver)" -c -o /work/fork/caf_lax_driver/LAX_host/driver/lax/build-linux-kernel/../os_agnostic/src//.tmp_cbuffer.o /work/fork/caf_lax_driver/LAX_host/driver/lax/build-linux-kernel/../os_agnostic/src//cbuffer.c

source_/work/fork/caf_lax_driver/LAX_host/driver/lax/build-linux-kernel/../os_agnostic/src//cbuffer.o := /work/fork/caf_lax_driver/LAX_host/driver/lax/build-linux-kernel/../os_agnostic/src//cbuffer.c

deps_/work/fork/caf_lax_driver/LAX_host/driver/lax/build-linux-kernel/../os_agnostic/src//cbuffer.o := \
  /work/fork/caf_lax_driver/LAX_host/driver/lax/os_agnostic/inc/cbuffer.h \
  /work/fork/caf_lax_driver/LAX_host/driver/lax/../oal/include/oal_utils.h \
  /work/fork/caf_lax_driver/LAX_host/driver/lax/../oal/include/common_stringify_macros.h \
  /work/fork/caf_lax_driver/LAX_host/driver/lax/../oal/include/linux/os_utils.h \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/panic/timeout.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /work/gcc-linaro-4.9-2015.05-x86_64_aarch64-linux-gnu/lib/gcc/aarch64-linux-gnu/4.9.3/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  include/uapi/linux/types.h \
  arch/arm64/include/generated/asm/types.h \
  include/uapi/asm-generic/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  arch/arm64/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
    $(wildcard include/config/64bit.h) \
  include/uapi/asm-generic/bitsperlong.h \
  include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  arch/arm64/include/uapi/asm/posix_types.h \
  include/uapi/asm-generic/posix_types.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  arch/arm64/include/asm/linkage.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
  include/linux/bitops.h \
  arch/arm64/include/asm/bitops.h \
  arch/arm64/include/asm/barrier.h \
    $(wildcard include/config/smp.h) \
  include/asm-generic/bitops/builtin-__ffs.h \
  include/asm-generic/bitops/builtin-ffs.h \
  include/asm-generic/bitops/builtin-__fls.h \
  include/asm-generic/bitops/builtin-fls.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/le.h \
  arch/arm64/include/uapi/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/uapi/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  arch/arm64/include/generated/asm/swab.h \
  include/uapi/asm-generic/swab.h \
  include/linux/byteorder/generic.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/typecheck.h \
  include/linux/printk.h \
    $(wildcard include/config/message/loglevel/default.h) \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
    $(wildcard include/config/lto.h) \
  include/linux/kern_levels.h \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/uapi/linux/kernel.h \
  include/uapi/linux/sysinfo.h \
  arch/arm64/include/asm/cache.h \
  arch/arm64/include/asm/cachetype.h \
  arch/arm64/include/asm/cputype.h \
  include/linux/dynamic_debug.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/uapi/linux/string.h \
  arch/arm64/include/asm/string.h \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  arch/arm64/include/generated/asm/errno.h \
  include/uapi/asm-generic/errno.h \
  include/uapi/asm-generic/errno-base.h \
  include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
  arch/arm64/include/generated/asm/bug.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
    $(wildcard include/config/preempt/rt/base.h) \
  include/linux/delay.h \
    $(wildcard include/config/preempt/rt/full.h) \
  arch/arm64/include/generated/asm/delay.h \
  include/asm-generic/delay.h \

/work/fork/caf_lax_driver/LAX_host/driver/lax/build-linux-kernel/../os_agnostic/src//cbuffer.o: $(deps_/work/fork/caf_lax_driver/LAX_host/driver/lax/build-linux-kernel/../os_agnostic/src//cbuffer.o)

$(deps_/work/fork/caf_lax_driver/LAX_host/driver/lax/build-linux-kernel/../os_agnostic/src//cbuffer.o):
