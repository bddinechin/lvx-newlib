/**
 * @file cache.h
 * @brief cache operation, data and instruction
 *
 * @section LICENSE
 *  Copyright (C) 2009-2024 Kalray SA.
 *  All rights reserved.
 */
#ifndef _LVX_CACHE_H
#  define _LVX_CACHE_H

#include <stdint.h>

#if defined(__open64__) || defined(__clang__)
#define __builtin_error(MSG) do { } while (0)
#else
#define __builtin_error(MSG) {                                          \
		extern int __attribute__((error(#MSG))) __builtin_compile_time_check(); \
		__builtin_compile_time_check();				\
	}
#endif /* defined(__open64__) || defined(__clang__) */

#define _LVX_DCACHE_LINE_SIZE    ((64*8)/8)
#define _LVX_DCACHE_SIZE (128*1024)

#define _LVX_ICACHE_LINE_SIZE    ((64*8)/8)
#define _LVX_ICACHE_SIZE (4*8*1024)

#ifndef _LVX_DCACHE_LINE_SIZE
#error  "Data Cache line size must be defined first!"
#endif
#ifndef _LVX_ICACHE_LINE_SIZE
#error  "Instruction Cache line size must be defined first!"
#endif

#define __COMPILER_MEMORY_BARRIER __asm__ ("":::"memory")

/*
 * Instruction Cache operations
 */

/**
 * \fn void __lvx_icache_enabled(void)
 * \brief Icache enabled ?
 * \return > 0 if icache is enabled, else 0
 */
extern unsigned int __lvx_icache_enabled(void);

/**
 * \fn void __lvx_icache_enable(void)
 * \brief Enable Instruction Cache
 */
extern void __lvx_icache_enable(void);

/**
 * \fn void __lvx_icache_disable(void)
 * \brief Disable Instruction Cache
 */
extern void __lvx_icache_disable(void);

/**
 * \fn void __lvx_icache_invalidate_mem_area(uintptr_t addr, int size)
 * \brief Invalidate a given memory area (addr + size) in instruction cache
 * \param addr Pointer (The memory area to invalidate will begin at
 * line that contains *addr)
 * \param size Size of the memory area to invalidate.
 * If the address + size overlap on a last instruction cache line,
 * this line will be invalidated too
 */
extern void __lvx_icache_invalidate_mem_area(uintptr_t addr, int size);

/*
 * Data Cache operations
 */

/**
 * \fn void __lvx_dcache_enabled(void)
 * \brief Dcache enabled ?
 * \return > 0 if dcache is enabled, else 0
 */
extern unsigned int __lvx_dcache_enabled(void);

/**
 * \fn void __lvx_dcache_enable(void)
 * \brief Enable data cache
 */
extern void __lvx_dcache_enable(void);

/**
 * \fn void __lvx_dcache_disable(void)
 * \brief Disable data cache
 */
extern void __lvx_dcache_disable(void);

/**
 * \fn void __lvx_dcache_invalidate_mem_area(uintptr_t addr, int size)
 * \brief Invalidate a given memory area (addr + size) in data cache
 * \param addr Pointer (The memory area to invalidate will begin at
 * line that contains *addr)
 * \param size Size of the memory area to invalidate.
 * If the address + size overlap on a last data cache line,
 * this line will be invalidated too
 */
extern void __lvx_dcache_invalidate_mem_area(uintptr_t addr, int size);

/**
 * \fn void __lvx_l1_coherency_enable(void)
 * \brief Enable l1 coherency on this lvx
 */
extern void __lvx_l1_coherency_enable(void);

/**
 * \fn void __lvx_l1_coherency_disable(void)
 * \brief Disable l1 coherency on this lvx
 */
extern void __lvx_l1_coherency_disable(void);

/**
 * \fn void __lvx_l2cache_enable(void)
 * \brief Enable data accesses from this lvx to be l2-cached
 */
extern void __lvx_l2cache_enable(void);

/**
 * \fn void __lvx_l2cache_disable(void)
 * \brief Disable data accesses from this lvx to be l2-cached
 */
extern void __lvx_l2cache_disable(void);

/** Full memory barrier **/
/**
 * \fn void __lvx_mb (void)
 * \brief Full memory barrier
 */
extern void __lvx_mb(void);

/** Write memory barrier **/
/**
 * \fn void __lvx_wmb (void)
 **\brief Write memory barrier
 */
extern void __lvx_wmb(void);

/** Read memory barrier **/
/**
 * \fn void __lvx_rmb (void)
 * \brief Read memory barrier
 */
void __lvx_rmb(void);

#endif      /* _LVX_CACHE_H */
