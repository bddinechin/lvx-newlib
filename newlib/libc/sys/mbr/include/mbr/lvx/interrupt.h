/**
 * @file interrupt.h
 * @brief Interrupts handling
 *
 * @section LICENSE
 *  Copyright (C) 2019-2026 Liesme Tech.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 3, or (at your option) any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  Under Section 7 of GPL version 3, you are granted additional
 *  permissions described in the GCC Runtime Library Exception, version
 *  3.1, as published by the Free Software Foundation.
 *
 *  You should have received a copy of the GNU General Public License and
 *  a copy of the GCC Runtime Library Exception along with this program;
 *  see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 * NOTE: this is a deliberately minimal subset of KVX's mppa_bare_runtime
 * interrupt.h - only what newlib/libgloss's timer.c actually needs
 * (enable/disable a single interrupt line by number, and the timer0 line
 * number). KVX's full interrupt.h also covers priority levels, interrupt
 * vector table management, and per-line status queries; port those here
 * too once something in this tree needs them.
 */

#ifndef _LVX_INTERRUPT_H
#  define _LVX_INTERRUPT_H

#    ifndef _LVX_RM_INT_LINE_TIMER0
#      define _LVX_RM_INT_LINE_TIMER0 0
#    endif

/**
 * \fn void __lvx_interrupt_enable_num(unsigned int x)
 * \brief Enable a given interrupt line
 * \param x Interrupt line number
 */
extern void __lvx_interrupt_enable_num(unsigned int x);

/**
 * \fn void __lvx_interrupt_disable_num(unsigned int x)
 * \brief Disable a given interrupt line
 * \param x Interrupt line number
 */
extern void __lvx_interrupt_disable_num(unsigned int x);

#endif      /* _LVX_INTERRUPT_H */
