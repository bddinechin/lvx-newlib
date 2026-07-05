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

#include <fenv.h>

int feraiseexcept(int excepts)
{
  /* Mask excepts to be sure only supported flag bits are set */
  excepts &= FE_ALL_EXCEPT;

  /* The wfxl insn allows to set the least significant word of the
   * operated system register (here $cs), with 'excepts << 32' as a
   * set mask.
   */
  asm volatile ("wfxl $cs, %0" : : "r"((long long)excepts << 32) : "$cs");

  /* C99 requirements are met. The flags are raised at the same time
   * so order is preserved. FE_INEXACT is not raised if one of the
   * exceptions is FE_OVERFLOW or FE_UNDERFLOW.
   */

  /* The above insn cannot fail (while the OS allows access to the
   * floating-point exception flags of the $cs register). Return
   * success.
   */
  return 0;
}
