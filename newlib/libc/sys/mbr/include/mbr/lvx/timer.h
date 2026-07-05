/**
 * @file timer.h
 * @brief timers and watchdog
 *
 * @section LICENSE
 *  Copyright (C) 2009-2024 Kalray SA.
 *  All rights reserved.
 *
 * NOTE: this is a deliberately minimal subset of KVX's mppa_bare_runtime
 * timer.h - only the four functions actually implemented by
 * libgloss/lvx-mbr/timer.c are declared here (KVX's timer.c also leaves
 * __kvx_timer_disable_num and the watchdog functions declared-but-unused,
 * so those were dropped rather than mirrored). power_ctrl.h isn't included
 * either: nothing in timer.c calls anything from it, and it pulls in
 * deeper undefined hardware headers (machine/pwr_ctrl.h, secure cluster
 * registers) that this repo has no LVX equivalent for yet.
 */

#ifndef _LVX_TIMER_H
#define _LVX_TIMER_H

#include "cpu.h"
#include "assert.h"
#include "interrupt.h"

#    ifndef _LVX_TIMER_NUMBER
#      define _LVX_TIMER_NUMBER 2
#    endif


/**
 * \fn void __lvx_timer_setup_num(unsigned int timer, unsigned int value, unsigned int reload, unsigned char itDisable)
 * \brief Setup a timer
 * \param timer Number of the timer to set up (0 or 1)
 * \param value Value to put in the timer (decremented on each tick)
 * \param reload Reload value (Loaded upon reaching 0)
 * \param itDisable disable timer interrupt
 */
extern void __lvx_timer_setup_num(unsigned int timer, unsigned long long value, unsigned long long reload, unsigned char itDisable);

/**
 * \fn void __lvx_timer_interrupt_enable_num(unsigned int timer)
 * \brief Enable Interrupt line for timer
 * \param timer timer number
 */
extern void __lvx_timer_interrupt_enable_num(unsigned int timer);

/**
 * \fn void __lvx_timer_interrupt_disable_num(unsigned int timer)
 * \brief Disable interrupt line for timer
 * \param timer timer number
 */
extern void __lvx_timer_interrupt_disable_num(unsigned int timer);

/**
 * \fn void __lvx_timer_get_value_num(unsigned int timer)
 * \brief get the current value of the timer
 * \param timer timer number
 * \return value of the timer
 */
extern unsigned int __lvx_timer_get_value_num(unsigned int timer);

#endif      /* _LVX_TIMER_H */
