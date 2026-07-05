/**
 * @file cpu.h
 * @brief Single core operations
 *
 * @section LICENSE
 *  Copyright (C) 2009-2024 Kalray SA.
 *  All rights reserved.
 *
 * NOTE: this is a deliberately minimal subset of KVX's mppa_bare_runtime
 * cpu.h - only the declarations/constants actually consumed elsewhere in
 * this repo (newlib/libc/sys/mbr/lock.c, libgloss/lvx-mbr/{times,timer}.c)
 * have been ported so far. KVX's cpu.h also covers hwloop enable/disable,
 * FPU rounding mode/exception control, and RTL-simulation bookkeeping
 * variables; port those here too once something in this tree needs them.
 *
 * _KVX_CPU_FREQ/_KVX_TIMESTAMP_FREQ are clock-speed assumptions tied to
 * Kalray's actual Coolidge silicon, not an ISA-level fact - there is no
 * real LVX hardware or ISS yet to measure. Reused verbatim from KVX here
 * (same policy as the syscall ABI in scall_no.h): internally-consistent
 * placeholders until a real LVX target defines its own timing.
 */

#ifndef _LVX_CPU_H
#  define _LVX_CPU_H

#include "registers.h"

/**
 * \fn int __lvx_get_cpu_id(void)
 * \brief Get the PE ID
 * \return PE ID
 */
extern int __lvx_get_cpu_id(void) __attribute__((__const__));

#    ifndef _LVX_CPU_FREQ
#      define _LVX_CPU_FREQ (800 * 1000 * 1000) /* 800 MHz, see NOTE above */
#    endif
#    ifndef __LVX_CLOCKS_PER_SEC__
#      define __LVX_CLOCKS_PER_SEC__ 1000000
#    endif
#    ifndef _LVX_TIMESTAMP_FREQ
#      define _LVX_TIMESTAMP_FREQ 100000000ULL
#    endif
#    ifndef _LVX_NANOSECONDS_PER_SECOND
#      define _LVX_NANOSECONDS_PER_SECOND (1000000000uLL)
#    endif

#endif      /* _LVX_CPU_H */
