/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_BOOT_H
#define _ASM_X86_BOOT_H


#include <asm/pgtable_types.h>
#include <uapi/asm/boot.h>

/* Physical address where kernel should be loaded. */
#define LOAD_PHYSICAL_ADDR ((CONFIG_PHYSICAL_START \
				+ (CONFIG_PHYSICAL_ALIGN - 1)) \
				& ~(CONFIG_PHYSICAL_ALIGN - 1))

/* Minimum kernel alignment, as a power of two */
#ifdef CONFIG_X86_64
# define MIN_KERNEL_ALIGN_LG2	PMD_SHIFT
#else
# define MIN_KERNEL_ALIGN_LG2	(PAGE_SHIFT + THREAD_SIZE_ORDER)
#endif
#define MIN_KERNEL_ALIGN	(_AC(1, UL) << MIN_KERNEL_ALIGN_LG2)

#if (CONFIG_PHYSICAL_ALIGN & (CONFIG_PHYSICAL_ALIGN-1)) || \
	(CONFIG_PHYSICAL_ALIGN < MIN_KERNEL_ALIGN)
# error "Invalid value for CONFIG_PHYSICAL_ALIGN"
#endif

#if defined(CONFIG_KERNEL_BZIP2)
# define BOOT_HEAP_SIZE		0x400000
#elif defined(CONFIG_KERNEL_ZSTD)
/*
 * Zstd needs to allocate the ZSTD_DCtx in order to decompress the kernel.
 * The ZSTD_DCtx is ~160KB, so set the heap size to 192KB because it is a
 * round number and to allow some slack.
 */
# define BOOT_HEAP_SIZE		 0x30000
#else
# define BOOT_HEAP_SIZE		 0x10000
#endif

#ifdef CONFIG_X86_64
# define BOOT_STACK_SIZE	0x4000

/*
 * Used by decompressor's startup_32() to allocate page tables for identity
 * mapping of the 4G of RAM in 4-level paging mode.
 *
 * The additional page table needed for 5-level paging is allocated from
 * trampoline_32bit memory.
 */
# define BOOT_INIT_PGT_SIZE	(6*4096)

/*
 * Total number of page table kernel_add_identity_map() can allocate,
 * including page tables consumed by startup_32().
 */
# define BOOT_PGT_SIZE		(32*4096)

#else /* !CONFIG_X86_64 */
# define BOOT_STACK_SIZE	0x1000
#endif

#ifndef __ASSEMBLY__
extern unsigned int output_len;
extern const unsigned long kernel_total_size;

unsigned long decompress_kernel(unsigned char *outbuf, unsigned long virt_addr,
				void (*error)(char *x));
#endif

#endif /* _ASM_X86_BOOT_H */
