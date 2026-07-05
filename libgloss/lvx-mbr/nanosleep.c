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

#include <errno.h>
#include <time.h>
#include <mbr/lvx/timestamp.h>
#include <mbr/lvx/cpu.h>
#include <sys/types.h>
#include <stdint.h>


/* _POSIX_TIMERS is not defined for bare. Internal routine __lvx_nanosleep is provided and used in sleep implementation. */
int __lvx_nanosleep(const struct timespec *req, struct timespec *rem)
{
  if (req) {
    uint64_t start  = __lvx_cluster_timestamp();
    uint64_t ticks  = (uint64_t)((req->tv_sec + req->tv_nsec /
      (float) _LVX_NANOSECONDS_PER_SECOND) * _LVX_TIMESTAMP_FREQ);

    while (ticks > (__lvx_cluster_timestamp() - start))
      ;

    if (rem) {
      rem->tv_nsec = rem->tv_sec = 0;
    }
  }
  return 0;
}
