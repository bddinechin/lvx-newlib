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


int
strcmp(const char *s1, const char *s2) {
    uint64_t has_null0;
    uint64_t has_null1;

    uint64x2_t *a1;
    uint64x2_t *a2;

    uint64x2_t vv1;
    uint64x2_t vv2;

    /* Compare strings 128 bits at a time until detecting a difference
        or a null terminator */
    a1 = (uint64x2_t *)s1;
    a2 = (uint64x2_t *)s2;
    do {
        /* Check for page crossing when doing unaligned loads */
        if (!PAGE_CROSS_SAFE2(uint64x2_t, a1, a2)) {
            char *s1_tmp = (char *)a1;
            char *s2_tmp = (char *)a2;

            /* Compare 1 byte at a time until the page has been crossed */
            do {
                unsigned char c1 = *s1_tmp++;
                unsigned char c2 = *s2_tmp++;
                if (c1 == '\0' || c1 != c2) {
                    return c1 - c2;
                }
            } while (!PAGE_CROSS_SAFE2(uint64x2_t, s1_tmp, s2_tmp));

            /* Update pointers */
            a1 = (uint64x2_t *)s1_tmp;
            a2 = (uint64x2_t *)s2_tmp;
        }

        vv1 = *a1++;
        vv2 = *a2++;
        has_null0 = DETECTNULL(vv1[0]);
        has_null1 = DETECTNULL(vv1[1]);
    } while ((vv1[0] == vv2[0] && vv1[1] == vv2[1]) &&
            !(has_null0 || has_null1));

    /* The strings are equal after iteration, return zero */
    if (vv1[0] == vv2[0] && vv1[1] == vv2[1] && (has_null0 || has_null1)) {
        return 0;
    }

    /* Edge case: The null terminator is stored in the last byte of
       the first 64 bits */
    if (vv1[0] == vv2[0] && has_null0) {
        return 0;
    }

    /* Otherwise count the trailing zeroes to determine where the
       first different bit is */
    uint64_t trail = cnt_trailzero((vv1[0] ^ vv2[0]) | has_null0);
    uint64_t v1 = vv1[0];
    uint64_t v2 = vv2[0];

    /* If the first 64 bits are equal, check the last 64 bits */
    if (vv1[0] == vv2[0]) {
        trail = cnt_trailzero((vv1[1] ^ vv2[1]) | has_null1);
        v1 = vv1[1];
        v2 = vv2[1];
    }

    /* Round to lower multiple of 8 to get the different byte */
    trail &= ~UINT64_C(7);

    const unsigned char c1 = v1 >> trail;
    const unsigned char c2 = v2 >> trail;

    return c1 - c2;
}
