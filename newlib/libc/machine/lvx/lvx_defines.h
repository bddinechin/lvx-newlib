/*
 * Copyright (C) 2019-2026 Liesme Tech.
 *
 * This file is part of Newlib, a C library for embedded systems.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * Under Section 7 of GPL version 3, you are granted additional
 * permissions described in the GCC Runtime Library Exception, version
 * 3.1, as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License and
 * a copy of the GCC Runtime Library Exception along with this program;
 * see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/* This header file contains various common macros and types and LVX builtins
   for use in LVX optimized implementations of libc functions */

#ifndef LVX_DEFINES_H
#define LVX_DEFINES_H

#include <stdint.h>


/* Helpers */

/* Check for a null terminator byte (64bits). Used in str functions.
   From https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
   The subexpression (~(X) & 0x8080808080808080) is replaced to the equivalent
   ~((X) | 0x7f7f7f7f7f7f7f7f) for better bundling. */
#define DETECTNULL(X) \
    (((X) - 0x0101010101010101) & ~((X) | 0x7f7f7f7f7f7f7f7f))

/* Nonzero if X is not aligned on a "type" boundary */
#define UNALIGNED(type, X) ((uintptr_t)X & (sizeof(type) - 1))
/* Nonzero if either X or Y is not aligned on a "type" boundary */
#define UNALIGNED2(type, X, Y) (UNALIGNED(type, X) | UNALIGNED(type, Y))
/* Create a mask to round down an address to a "type" boundary */
#define ALIGN_MASK(type) (~(sizeof(type) - 1))


/* Smallest page size */
#define PAGE_SIZE (4096)
/* Zero if X will cross pages on load of "type" */
#define PAGE_CROSS_SAFE(type, X) \
    (((uintptr_t)X & (PAGE_SIZE - sizeof(type))) ^ (PAGE_SIZE - sizeof(type)))
/* Zero if either X or Y will cross pages on load of "type" */
#define PAGE_CROSS_SAFE2(type, X, Y) \
    (PAGE_CROSS_SAFE(type, X) | PAGE_CROSS_SAFE(type, Y))

#define unlikely(cond) __builtin_expect(!!(cond), 0)
#define likely(cond) __builtin_expect(!!(cond), 1)

/* Vector types */
typedef uint64_t uint64x2_t __attribute__((vector_size(2 * sizeof(uint64_t))));
typedef uint64_t uint64x4_t __attribute__((vector_size(4 * sizeof(uint64_t))));


/* LVX builtins */

/* Count trailing zeroes (64bits) */
#define cnt_trailzero(a) __builtin_lvx_bitcntd(a, ".tz")

#endif /* LVX_DEFINES_H */
