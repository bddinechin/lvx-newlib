/**
 * @file diagnostic.h
 * @brief Debug and profile operations
 *
 * @section LICENSE
 *  Copyright (C) 2009-2024 Kalray SA.
 *  All rights reserved.
 *
 * NOTE: this is a deliberately minimal subset of KVX's mppa_bare_runtime
 * diagnostic.h - only __lvx_counter_num is declared, since that's the one
 * symbol libgloss/lvx-mbr/times.c actually uses (to read performance
 * counter 0, assumed by convention to already be counting processor
 * clock cycles out of reset - see KVX's _KVX_PM_PCC). KVX's full
 * diagnostic.h also covers counter start/stop/configuration for a dozen
 * other metrics; port those here too once something in this tree needs
 * them.
 */

#ifndef _LVX_DIAGNOSTIC_H
#  define _LVX_DIAGNOSTIC_H

#include <stdint.h>

/**
 * \fn uint64_t __lvx_counter_num(unsigned int n)
 * \brief Get value of counters
 * \param n Number of counter
 * \return Content of counter
 */
extern uint64_t __lvx_counter_num(unsigned int n);

#endif /* _LVX_DIAGNOSTIC_H */
