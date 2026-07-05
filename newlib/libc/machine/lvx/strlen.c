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

#include "lvx_defines.h"

#include <stddef.h>

size_t
strlen(const char *str) {
    uint64x4_t *a256 = (uint64x4_t *)str;
    uint64x4_t v256;

    /* Do a 256bits unaligned prepass first if no page cross will occur */
    if (PAGE_CROSS_SAFE(uint64x4_t, a256)) {
        v256 = *a256;
        size_t len = ((char *)a256) - str;

        /* Check for null terminators 64 bits at a time */
        uint64_t has_null0 = DETECTNULL(v256[0]);
        uint64_t has_null1 = DETECTNULL(v256[1]);
        uint64_t has_null2 = DETECTNULL(v256[2]);
        uint64_t has_null3 = DETECTNULL(v256[3]);

        /* Get the offset of a potential null terminator in each 64 bits */
        uint64_t offset0 = cnt_trailzero(has_null0);
        uint64_t offset1 = cnt_trailzero(has_null1);
        uint64_t offset2 = cnt_trailzero(has_null2);
        uint64_t offset3 = cnt_trailzero(has_null3);

        /* Return the length of the string if a null terminator was found */
        if (has_null0) {
            return len + (offset0 / 8);
        }
        if (has_null1) {
            return len + (offset1 / 8) + 8;
        }
        if (has_null2) {
            return len + (offset2 / 8) + 16;
        }
        if (has_null3) {
            return len + (offset3 / 8) + 24;
        }

        a256++;

        /* Round down address to 32 bytes alignment, which means we will be
            checking some previous bytes twice, but remove page cross check
            from the main loop */
        a256 = (uint64x4_t *)((uintptr_t)a256 & ALIGN_MASK(uint64x4_t));

    /* Page cross on the first load is rare, so read 1 byte at a time until
       the pointer is aligned.
       This could be improved by loading from a lower 32 bytes aligned address
       and mask preceding bytes to 0x7F (non '\0'), then go to main loop */
    } else {
        char *a8 = (char *)a256;

        /* Iterate 1 byte at a time until the page has been crossed */
        do {
            char c = *a8++;
            if (c == '\0') {
                return --a8 - str;
            }
        } while(!PAGE_CROSS_SAFE(uint64x4_t, a8));

        /* Update pointer */
        a256 = (uint64x4_t *)a8;
    }


    /* Main aligned loop */
    do {
        v256 = *a256;
        size_t len = ((char *)a256) - str;

        /* Check for null terminators 64 bits at a time */
        uint64_t has_null0 = DETECTNULL(v256[0]);
        uint64_t has_null1 = DETECTNULL(v256[1]);
        uint64_t has_null2 = DETECTNULL(v256[2]);
        uint64_t has_null3 = DETECTNULL(v256[3]);

        /* Get the offset of a potential null terminator in each 64 bits */
        uint64_t offset0 = cnt_trailzero(has_null0);
        uint64_t offset1 = cnt_trailzero(has_null1);
        uint64_t offset2 = cnt_trailzero(has_null2);
        uint64_t offset3 = cnt_trailzero(has_null3);

        /* Return the length of the string when a null terminator was found */
        if (unlikely(has_null0)) {
            return len + (offset0 / 8);
        }
        if (unlikely(has_null1)) {
            return len + (offset1 / 8) + 8;
        }
        if (unlikely(has_null2)) {
            return len + (offset2 / 8) + 16;
        }
        if (unlikely(has_null3)) {
            return len + (offset3 / 8) + 24;
        }

        a256++;
    } while (1);
}
