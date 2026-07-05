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

#include "errno.h"
#include <sys/time.h>
#include <time.h>

#include <mbr/lvx/timestamp.h>
#include <mbr/lvx/cpu.h>

#define NANOSEC_PER_SECOND (1000000000uLL)

int __lvx_getnanotime(struct timespec *tval)
{
  uint64_t ticks = 0;
  uint64_t cycles;
  uint64_t end;

  cycles  = __lvx_cluster_timestamp();
  tval->tv_sec = cycles / _LVX_TIMESTAMP_FREQ;
  tval->tv_nsec = (cycles -  _LVX_TIMESTAMP_FREQ*(uint64_t)tval->tv_sec) * (NANOSEC_PER_SECOND/(float)_LVX_TIMESTAMP_FREQ);

  return 0;
}

int _gettimeofday(struct timeval *tval, void *tz)
{
  struct timespec tp;
  if(!tval){
    errno = EINVAL;
    return -1;
  }
  __lvx_getnanotime(&tp);
  tval->tv_sec = tp.tv_sec;
  tval->tv_usec = (tp.tv_nsec / 1000);
  return 0;
}

int gettimeofday(struct timeval *tval, void *tz) __attribute__ ((weak, alias ("_gettimeofday")));
