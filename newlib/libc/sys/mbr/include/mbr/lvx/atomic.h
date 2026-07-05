/**
 * @file atomic.h
 * @brief Atomic operations, locks
 *
 * @section LICENSE
 *  Copyright (C) 2009-2024 Kalray SA.
 *  All rights reserved.
 */
#ifndef _LVX_ATOMIC_H
#define _LVX_ATOMIC_H

#include "cache.h"
#include <stdint.h>

typedef union {
	struct {
		uint64_t new_value;	/* update */
		uint64_t old_value;	/* expect */
	};
	__int128 qword;
} __lvx_swap_value_t;

/**
 * \brief Atomically swap a 32bits value in memory with a value passed as a parameter.
 * \param addr memory address to set
 * \param new_value new value to set
 * \return old value in memory
 */
extern uint32_t __lvx_atomic_swap32(uint32_t *addr, uint32_t new_value);

/**
 * \brief Atomically swap a 64bits value in memory with a value passed as a parameter.
 * \param addr memory address to set
 * \param new_value new value to set
 * \return old value in memory
 */
extern uint64_t __lvx_atomic_swap64(uint64_t *addr, uint64_t new_value);

/**
 * \brief Atomically add a 32bits value in memory
 * \param addr memory address to increment
 * \param value value to add
 * \return the value that had previously been in memory
 */
extern int32_t __lvx_atomic_add32(uint32_t *addr, int32_t value);

/**
 * \brief Atomically compare 32bits old_value to current memory value and swap with
 * new_value if equal
 * \return 1 on success
 * 0 if old_value not equal
 */
extern int __lvx_compare_and_swap32(void *ptr, uint32_t old_value,
				   uint32_t new_value);

// ============ Backoff spinlock ============= //

/**
 * \struct __lvx_recursive_lock_t
 * \brief Recursive Locks
 *
 * Recursive Locks (SMP ready)
 *
 * WARNING : an owner value of 0x0 means "unlocked"
 *
 * Principle: This lock can be recursively locked by the same owner.
 *            On lock we CAS for being sure that it is either ours or unlocked.
 */
typedef struct __lvx_recursive_lock __lvx_recursive_lock_t;

struct __lvx_recursive_lock {
	uint32_t owner;
	uint32_t counter;
};

#ifndef _LVX_RECURSIVE_NO_OWNER
#define _LVX_RECURSIVE_NO_OWNER 0x0UL
#endif				/* _LVX_RECURSIVE_NO_OWNER  */

#ifndef _LVX_RECURSIVE_LOCK_UNLOCKED
#define _LVX_RECURSIVE_LOCK_UNLOCKED {.owner = _LVX_RECURSIVE_NO_OWNER, .counter = 0}
#endif				/* _LVX_RECURSIVE_LOCK_UNLOCKED  */

/**
 * \fn void __lvx_recursive_initlock_base(__lvx_recursive_lock_t *lock)
 * \brief Initialize recursive lock
 * \param lock Pointer to recursive lock struct
 * \warning Cache line containing lock struct is flushed
 */
extern void __lvx_recursive_initlock_base(__lvx_recursive_lock_t *lock);

/**
 * \fn int __lvx_recursive_trylock_base(__lvx_recursive_lock_t *lock,
 *                                     uint64_t myself)
 * \brief Try to acquire recursive lock
 * This function tries to acquire recursive lock. It does not perform any
 * cache operations.
 * \param lock Pointer to recursive lock struct
 * \param myself My owner value. WARNING 0x0 means unlocked
 * \return 1 if lock is acquired, else 0
 * \warning The cache line containing the lock counter is flushed
 */
extern int __lvx_recursive_trylock_base(__lvx_recursive_lock_t *lock,
				       uint32_t myself);

/**
 * \fn void __lvx_recursive_lock_base(__lvx_recursive_lock_t *lock,
 *                                   uint64_t myself)
 * \brief Loop until recursive lock is acquired
 * This version has no effect on cache.
 * \param lock Pointer to recursive lock struct
 */
extern void __lvx_recursive_lock_base(__lvx_recursive_lock_t *lock,
				     uint64_t myself);

/**
 * \fn void __lvx_recursive_unlock_base(__lvx_recursive_lock_t *lock,
 *                                     uint64_t myself)
 * \brief Release recursive lock
 * This function unlocks an recursive lock. It does not perform any
 * cache oeprations.
 * \param lock Pointer to recursive lock struct
 * \param myself My owner value. WARNING 0x0 means unlocked
 */
extern int __lvx_recursive_unlock_base(__lvx_recursive_lock_t *lock,
				      uint64_t myself);

/**
 * \fn void __lvx_recursive_release_base(__lvx_recursive_lock_t *lock,
 *                                      uint64_t myself)
 * \brief Release recursive lock by dropping all levels
 * This function releases a recursive lock. It does not perform any
 * cache operations.
 * \param lock Pointer to recursive lock struct
 * \param myself My owner value. WARNING 0x0 means unlocked
 */
extern int __lvx_recursive_release_base(__lvx_recursive_lock_t *lock,
				       uint64_t myself);

#endif				/* _LVX_ATOMIC_H */
