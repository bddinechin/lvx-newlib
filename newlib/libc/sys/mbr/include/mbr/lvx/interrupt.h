/**
 * @file interrupt.h
 * @brief Interrupts handling
 *
 * @section LICENSE
 *  Copyright (C) 2009-2024 Kalray SA.
 *  All rights reserved.
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
