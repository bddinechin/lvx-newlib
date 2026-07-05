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

#include <mbr/lvx/timer.h>

void __lvx_timer_setup_num(unsigned int timer, unsigned long long value, unsigned long long reload, unsigned char itDisable)
{

	__lvx_assert(timer < _LVX_TIMER_NUMBER);

	switch (timer) {
	case 0:
		/* setup value */
		__builtin_lvx_set(LVX_SFR_T0R, reload);
		__builtin_lvx_set(LVX_SFR_T0V, value);
		/* clear status just in case */
		__builtin_lvx_wfxl(LVX_SFR_TCR, LVX_SFR_TCR_T0ST_WFXL_CLEAR);
		if (!itDisable) {
			/* enable interrupt if asked to*/
			__builtin_lvx_wfxl(LVX_SFR_TCR, LVX_SFR_TCR_T0IE_WFXL_SET);
		}
		/* go ! */
		__builtin_lvx_wfxl(LVX_SFR_TCR, LVX_SFR_TCR_T0CE_WFXL_SET);
		break;
	case 1:
		/* setup value */
		__builtin_lvx_set(LVX_SFR_T1R, reload);
		__builtin_lvx_set(LVX_SFR_T1V, value);
		/* clear status just in case */
		__builtin_lvx_wfxl(LVX_SFR_TCR, LVX_SFR_TCR_T1ST_WFXL_CLEAR);
		if (!itDisable) {
			/* enable interrupt if asked to*/
			__builtin_lvx_wfxl(LVX_SFR_TCR, LVX_SFR_TCR_T1IE_WFXL_SET);
		}
		/* go ! */
		__builtin_lvx_wfxl(LVX_SFR_TCR, LVX_SFR_TCR_T1CE_WFXL_SET);
		break;
	default:
		__lvx_assert(0);
	}

}

void __lvx_timer_interrupt_enable_num(unsigned int timer)
{

	__lvx_assert(timer < _LVX_TIMER_NUMBER);
	__lvx_interrupt_enable_num(_LVX_RM_INT_LINE_TIMER0 + timer);
}

void __lvx_timer_interrupt_disable_num(unsigned int timer)
{
	__lvx_assert(timer < _LVX_TIMER_NUMBER);
	__lvx_interrupt_disable_num(_LVX_RM_INT_LINE_TIMER0 + timer);
}

unsigned int __lvx_timer_get_value_num(unsigned int timer)
{

	__lvx_assert(timer < _LVX_TIMER_NUMBER);

	switch (timer) {
	case 0:
		return __builtin_lvx_get(LVX_SFR_T0V);
	case 1:
		return __builtin_lvx_get(LVX_SFR_T1V);
	default:
		return 0;
	}
	return 0;
}
