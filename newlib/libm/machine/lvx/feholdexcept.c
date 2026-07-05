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

int feholdexcept(fenv_t *envp)
{
  /* Get the current environment ($cs) */
  fenv_t fe;
  asm volatile("get %0 = $cs" : "=r"(fe));

  /* Mask $cs status to keep exception flags and rounding mode only. */
  *envp = (fe & (FE_ALL_EXCEPT | FE_RND_MASK));

  /* The wfxl insn allows to set the least significant word of the
   * operated system register (here $cs), with 'FE_ALL_EXCEPT' as a
   * clear mask.
   */
  asm volatile ("wfxl $cs, %0" : : "r"(FE_ALL_EXCEPT) : "$cs");

  /* K1 does not raise FP traps so it is always in a "non-stop" mode */

  /* The above insn cannot fail (while the OS allows access to the
   * floating-point exception flags of the $cs register). Return
   * success.
   */
  return 0;
}
