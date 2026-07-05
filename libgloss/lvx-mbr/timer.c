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
