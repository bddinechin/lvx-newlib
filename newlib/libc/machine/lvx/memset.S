/*
 * Copyright 2019
 * Kalray Inc. All rights reserved.
 *
 * This software is furnished under license and may be used and copied only
 * in accordance with the following terms and conditions.  Subject to these
 * conditions, you may download, copy, install, use, modify and distribute
 * modified or unmodified copies of this software in source and/or binary
 * form. No title or ownership is transferred hereby.
 *
 * 1) Any source code used, modified or distributed must reproduce and
 *    retain this copyright notice and list of conditions as they appear in
 *    the source file.
 *
 * 2) No right is granted to use any trade name, trademark, or logo of
 *    Kalray Inc.  The "Kalray Inc" name may not be
 *    used to endorse or promote products derived from this software
 *    without the prior written permission of Kalray Inc.
 *
 * 3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR IMPLIED
 *    WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED WARRANTIES OF
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
 *    NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT SHALL KALRAY BE LIABLE
 *    FOR ANY DAMAGES WHATSOEVER, AND IN PARTICULAR, KALRAY SHALL NOT BE
 *    LIABLE FOR DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *    BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *    OR OTHERWISE), EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "asm.h"

#define REPLICATE_BYTE_MASK	0x0101010101010101
#define MIN_SIZE_FOR_ALIGN	128

/*
 * Optimized memset for lvx architecture
 *
 * In order to optimize memset on lvx, we can use various things:
 * - conditionnal store which avoid branch penalty
 * - store half/word/double/quad/octuple to store up to 32 bytes at a time
 * - hardware loop for steady cases.
 *
 * There is no cache-line zeroing instruction (no equivalent of KVX kv3-1's
 * 'dzerol') on lvx-1, so unlike KVX we don't special-case the zero pattern:
 * we always replicate the pattern byte on all bits of a register with sbmm8d
 * and go through the same aligned "so" store loop regardless of whether the
 * pattern is zero or not.
 *
 * First, we check if the size is below a minimum size. If so, we skip the
 * alignment part. Indeed, lvx supports misalignment and the penalty for
 * letting it do unaligned accesses is lower than trying to realign us. So
 * for small sizes, we don't even bother to realign.
 * Once alignment has been reached, we can do the hardware loop (store
 * octuple) in order to optimize throughput. Care must be taken to align
 * hardware loops on at least 8 bytes for performances.
 * Once the main loop has been done, we finish the copy by checking length to
 * do the necessary calls to store remaining bytes.
 */

.text
.global memset
.proc memset
.align 16
memset:
.align 16
.Lmemset_non_0_pattern:
	/* Preserve return value */
	copyd $r3 = $r0
	/* Replicate the first pattern byte on all bytes */
	sbmm8d $r32 = $r1, REPLICATE_BYTE_MASK
	/* Check if length < MIN_SIZE_FOR_ALIGN */
	compd.geu $r7 = $r2, MIN_SIZE_FOR_ALIGN
	/* Invert address to compute what we need to copy to be aligned on 32 bytes */
	negd $r5 = $r0
	;;
	/* Check if we are aligned on 32 bytes */
	andd $r9 = $r0, 0x1F
	/* Compute the length that will be copied to align on 32 bytes boundary */
	andd $r6 = $r5, 0x1F
	/*
	 * If size < MIN_SIZE_FOR_ALIGN bits, directly go to so, it will be done
	 * unaligned but that is still better that what we can do with sb
	 */
	cb.deqz $r7? .Laligned_32
	;;
	/* Remove unaligned part from length */
	sbfd $r2 = $r6, $r2
	/* If we are already aligned on 32 bytes, jump to main "so" loop */
	cb.deqz $r9? .Laligned_32
	/* Check if we need to copy 1 byte */
	andd $r4 = $r5, (1 << 0)
	;;
	/* If we are not aligned, store byte */
	stcond b, .dnez, $r4, 0[$r0], $r32
	/* Check if we need to copy 2 bytes */
	andd $r4 = $r5, (1 << 1)
	/* Add potentially copied part for next store offset */
	addd $r0 = $r0, $r4
	;;
	stcond h, .dnez, $r4, 0[$r0], $r32
	/* Check if we need to copy 4 bytes */
	andd $r4 = $r5, (1 << 2)
	addd $r0 = $r0, $r4
	;;
	stcond w, .dnez, $r4, 0[$r0], $r32
	/* Check if we need to copy 8 bytes */
	andd $r4 = $r5, (1 << 3)
	addd $r0 = $r0, $r4
	/* Copy second part of pattern for sq */
	copyd $r33 = $r32
	;;
	stcond d, .dnez, $r4, 0[$r0], $r32
	/* Check if we need to copy 16 bytes */
	andd $r4 = $r5, (1 << 4)
	addd $r0 = $r0, $r4
	;;
	stcond q, .dnez, $r4, 0[$r0], $r32r33
	addd $r0 = $r0, $r4
	;;
.Laligned_32:
	/* Copy second part of pattern for sq */
	copyd $r33 = $r32
	/* Prepare amount of data for 32 bytes store */
	srld $r10 = $r2, 5
	nop
	nop
	;;
	copyq $r34r35 = $r32, $r33
	/* Remaining bytes for 16 bytes store */
	andd $r8 = $r2, (1 << 4)
	make $r11 = 32
	/* Check if there are enough data for 32 bytes store */
	cb.deqz $r10? .Laligned_32_done
	;;
	loopdo $r10, .Laligned_32_done
		;;
		so 0[$r0] = $r32r33r34r35
		addd $r0 = $r0, $r11
		;;
	.Laligned_32_done:
	/*
	 * Now that we have handled every aligned bytes using 'so', we can
	 * handled the remainder of length using store by decrementing size
	 * We also exploit the fact we are aligned to simply check remaining
	 * size */
	stcond q, .dnez, $r8, 0[$r0], $r32r33
	addd $r0 = $r0, $r8
	/* Remaining bytes for 8 bytes store */
	andd $r8 = $r2, (1 << 3)
	cb.deqz $r2? .Lmemset_done
	;;
	stcond d, .dnez, $r8, 0[$r0], $r32
	addd $r0 = $r0, $r8
	/* Remaining bytes for 4 bytes store */
	andd $r8 = $r2, (1 << 2)
	;;
	stcond w, .dnez, $r8, 0[$r0], $r32
	addd $r0 = $r0, $r8
	/* Remaining bytes for 2 bytes store */
	andd $r8 = $r2, (1 << 1)
	;;
	stcond h, .dnez, $r8, 0[$r0], $r32
	addd $r0 = $r0, $r8
	;;
	stcond b, .odd, $r2, 0[$r0], $r32
	/* Restore original value */
	copyd $r0 = $r3
	ret
	;;
.Lmemset_done:
	/* Restore original value */
	copyd $r0 = $r3
	ret
	;;
.endp memset
