/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _X86_CRASH_CORE_H
#define _X86_CRASH_CORE_H

/* 16M alignment for crash kernel regions */
#define CRASH_ALIGN             SZ_16M

/*
 * Keep the crash kernel below this limit.
 *
 * Earlier 32-bits kernels would limit the kernel to the low 512 MB range
 * due to mapping restrictions.
 *
 * 64-bit kdump kernels need to be restricted to be under 64 TB, which is
 * the upper limit of system RAM in 4-level paging mode. Since the kdump
 * jump could be from 5-level paging to 4-level paging, the jump will fail if
 * the kernel is put above 64 TB, and during the 1st kernel bootup there's
 * no good way to detect the paging mode of the target kernel which will be
 * loaded for dumping.
 */

#ifdef CONFIG_X86_32
# define CRASH_ADDR_LOW_MAX     SZ_512M
# define CRASH_ADDR_HIGH_MAX    SZ_512M
#else
# define CRASH_ADDR_LOW_MAX     SZ_4G
# define CRASH_ADDR_HIGH_MAX    SZ_64T
#endif

# define DEFAULT_CRASH_KERNEL_LOW_SIZE crash_low_size_default()

extern unsigned long crash_low_size_default(void);

#endif /* _X86_CRASH_CORE_H */
