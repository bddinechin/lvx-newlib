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

#include <sys/types.h>
#include <sys/times.h>
#include <errno.h>

#include <mbr/lvx/cpu.h>
#include <mbr/lvx/diagnostic.h>

clock_t
_times (struct tms *buf)
{
  /*  errno = ENOSYS; */
  int cycle_ratio = _LVX_CPU_FREQ / __LVX_CLOCKS_PER_SEC__;
  clock_t cycles = __lvx_counter_num(0)/ cycle_ratio;
  /* User time */
  buf->tms_utime = cycles;
  /* System time */
  buf->tms_stime = 0;
  /* children's user time */
  buf->tms_cutime = 0;
  /* children's system time */
  buf->tms_cstime = 0;
  return cycles;
}

clock_t times (struct tms *buf)
  __attribute__ ((weak, alias ("_times")));
