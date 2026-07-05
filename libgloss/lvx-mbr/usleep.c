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
#include <unistd.h>

#define TOD_MICROSECONDS_PER_SECOND     1000000

extern int __lvx_nanosleep(const struct timespec *req, struct timespec *rem);
int usleep(useconds_t useconds){
  struct timespec t;

  t.tv_sec = useconds / TOD_MICROSECONDS_PER_SECOND;
  t.tv_nsec = (useconds % TOD_MICROSECONDS_PER_SECOND) * 1000;

  return __lvx_nanosleep(&t, NULL);

}
