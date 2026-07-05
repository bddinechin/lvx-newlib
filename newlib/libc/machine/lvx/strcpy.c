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

/* Copy remaining bytes from the last 64bits ('\0' included) */
static inline void
copylast64(uint64_t last, uint64_t *dest, uint64_t offset) {
    union {
        uint64_t *d;
        uint32_t *w;
        uint16_t *h;
        uint8_t  *b;
    } dstptr = { .d = dest };

#ifndef __llvm__

    /* 8 remaining bytes */
    uint64_t cond8 = offset == 64;
    __builtin_lvx_storecd(last, dstptr.d, cond8, ".dnez");
    /* We could technically return here if cond8 is true, but this
       currently adds a branching that would slow down other cases */

    /* 4-7 remaining bytes */
    uint64_t cond4_7 = offset >= 32;
    __builtin_lvx_storecw(last, dstptr.w, cond4_7, ".dnez");
    if (cond4_7) {
        offset -= 32;
        dstptr.w++;
        last >>= 32;
    }

    /* 2-3 remaining bytes */
    uint64_t cond2_3 = offset >= 16;
    __builtin_lvx_storech(last, dstptr.h, cond2_3, ".dnez");
    if (cond2_3) {
        offset -= 16;
        dstptr.h++;
        last >>= 16;
    }

    /* Write the last remaining byte */
    uint64_t cond1 = offset == 8;
    __builtin_lvx_storecb(last, dstptr.b, cond1, ".dnez");

#else
    /* LLVM doesn't support conditional store builtins yet,
       copy byte per byte */
    for (uint64_t i = 0; i < offset; i+=8) {
        *dstptr.b++ = last;
        last >>= 8;
    }

#endif
}


char *
strcpy(char *dst, const char *src) {
    uint64x4_t *src256 = (uint64x4_t *)src;
    uint64x4_t *dst256 = (uint64x4_t *)dst;
    uint64x4_t v256;

    /* Do a 256 bits prepass if no page cross will occur */
    if (PAGE_CROSS_SAFE(uint64x4_t, src256)) {
        v256 = *src256;

        /* Check for null terminators 64 bits at a time */
        uint64_t has_null0 = DETECTNULL(v256[0]);
        uint64_t has_null1 = DETECTNULL(v256[1]);
        uint64_t has_null2 = DETECTNULL(v256[2]);
        uint64_t has_null3 = DETECTNULL(v256[3]);

         /* Offset is rounded up to a multiple of 8 by adding one */
        uint64_t offset0 = (cnt_trailzero(has_null0) + 1);
        uint64_t offset1 = (cnt_trailzero(has_null1) + 1);
        uint64_t offset2 = (cnt_trailzero(has_null2) + 1);
        uint64_t offset3 = (cnt_trailzero(has_null3) + 1);

        if (has_null0) {
            copylast64(v256[0], (uint64_t *)dst256, offset0);
            return dst;

        } else if (has_null1) {
            /* Copy first 8 bytes */
            uint64_t *dst64 = (uint64_t *)dst256;
            *dst64++ = v256[0];

            copylast64(v256[1], dst64, offset1);
            return dst;

        } else if (has_null2) {
            /* Copy first 16 bytes */
            uint64x2_t *dst128 = (uint64x2_t *)dst256;
            *dst128++ = (uint64x2_t){v256[0], v256[1]};

            copylast64(v256[2], (uint64_t *)dst128, offset2);
            return dst;

        } else if (has_null3) {
            /* Copy first 24 bytes */
            uint64x2_t *dst128 = (uint64x2_t *)dst256;
            *dst128++ = (uint64x2_t){v256[0], v256[1]};

            uint64_t *dst64 = (uint64_t *)dst128;
            *dst64++ = v256[2];

            copylast64(v256[3], dst64, offset3);
            return dst;

        } else {
            /* Copy 256bits and continue */
            *dst256++ = v256;
            src256++;
        }

        /* Align src256 to lower 256bits while keeping alignment with dst256 */
        uint64_t align_offset = UNALIGNED(uint64x4_t, src256);
        src256 = (uint64x4_t *)((uintptr_t)src256 - align_offset);
        dst256 = (uint64x4_t *)((uintptr_t)dst256 - align_offset);


    /* Page cross on the first load is rare, so read 1 byte at a time until
       the pointer is aligned.
       This could be improved by loading from a lower 32 bytes aligned address
       and masking/not copying the preceding bytes */
    } else {
        unsigned char *src8 = (unsigned char *)src256;
        unsigned char *dst8 = (unsigned char *)dst256;

        do {
            unsigned char c = *src8++;
            *dst8++ = c;
            if (c == '\0') {
                return dst;
            }
        } while(!PAGE_CROSS_SAFE(uint64x4_t, src8));

        src256 = (uint64x4_t *)src8;
        dst256 = (uint64x4_t *)dst8;
    }

    /* Copy string 256bits at a time up to a null terminator */
    do {
        v256 = *src256;

        /* Check for null terminators 64 bits at a time */
        uint64_t has_null0 = DETECTNULL(v256[0]);
        uint64_t has_null1 = DETECTNULL(v256[1]);
        uint64_t has_null2 = DETECTNULL(v256[2]);
        uint64_t has_null3 = DETECTNULL(v256[3]);

        /* Offset is rounded up to a multiple of 8 by adding one */
        uint64_t offset0 = (cnt_trailzero(has_null0) + 1);
        uint64_t offset1 = (cnt_trailzero(has_null1) + 1);
        uint64_t offset2 = (cnt_trailzero(has_null2) + 1);
        uint64_t offset3 = (cnt_trailzero(has_null3) + 1);

        if (unlikely(has_null0)) {
            copylast64(v256[0], (uint64_t *)dst256, offset0);
            return dst;

        } else if (unlikely(has_null1)) {
            /* Copy first 8 bytes */
            uint64_t *dst64 = (uint64_t *)dst256;
            *dst64++ = v256[0];

            copylast64(v256[1], (uint64_t *)dst64, offset1);
            return dst;

        } else if (unlikely(has_null2)) {
            /* Copy first 16 bytes */
            uint64x2_t *dst128 = (uint64x2_t *)dst256;
            *dst128++ = (uint64x2_t){v256[0], v256[1]};

            copylast64(v256[2], (uint64_t *)dst128, offset2);
            return dst;

        } else if (unlikely(has_null3)) {
            /* Copy first 24 bytes */
            uint64x2_t *dst128 = (uint64x2_t *)dst256;
            *dst128++ = (uint64x2_t){v256[0], v256[1]};

            uint64_t *dst64 = (uint64_t *)dst128;
            *dst64++ = v256[2];

            copylast64(v256[3], (uint64_t *)dst64, offset3);
            return dst;

        } else {
            /* Copy 256bits and continue */
            *dst256++ = v256;
            src256++;
        }
    } while (1);
}
