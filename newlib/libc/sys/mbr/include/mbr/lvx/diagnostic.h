/**
 * @file diagnostic.h
 * @brief Debug and profile operations
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
