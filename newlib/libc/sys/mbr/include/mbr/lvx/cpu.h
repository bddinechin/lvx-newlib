/**
 * @file cpu.h
 * @brief Single core operations
 *
 * @section LICENSE
 *  Copyright (C) 2009-2024 Kalray SA.
 *  All rights reserved.
 *
 * NOTE: this is a deliberately minimal subset of KVX's mppa_bare_runtime
 * cpu.h - only the declaration(s) actually consumed elsewhere in this repo
 * (newlib/libc/sys/mbr/lock.c) have been ported so far. KVX's cpu.h also
 * covers hwloop enable/disable, FPU rounding mode/exception control, and
 * RTL-simulation bookkeeping variables; port those here too once something
 * in this tree needs them.
 */

#ifndef _LVX_CPU_H
#  define _LVX_CPU_H

/**
 * \fn int __lvx_get_cpu_id(void)
 * \brief Get the PE ID
 * \return PE ID
 */
extern int __lvx_get_cpu_id(void) __attribute__((__const__));

#endif      /* _LVX_CPU_H */
