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

#include <mbr/lvx/scall_no.h>

#include <fcntl.h>
#include <stdarg.h>
#include "errno.h"

int sc_open(const char *name,int flags, int mode);

int _open(const char *name,int flags, int mode)
{
  int ret = 0;
  int oflags = 0;

  if (flags & O_RDONLY)   oflags |= S_RDONLY;
  if (flags & O_WRONLY)   oflags |= S_WRONLY;
  if (flags & O_RDWR)     oflags |= S_RDWR;
  if (flags & O_APPEND)   oflags |= S_APPEND;
  if (flags & O_CREAT)    oflags |= S_CREAT;
  if (flags & O_TRUNC)    oflags |= S_TRUNC;
  if (flags & O_EXCL)     oflags |= S_EXCL;
  if (flags & O_SYNC)     oflags |= S_SYNC;
#ifdef O_NDELAY
  if (flags & O_NDELAY)   oflags |= S_NDELAY;
#endif
  if (flags & O_NONBLOCK) oflags |= S_NONBLOCK;
  if (flags & O_NOCTTY)   oflags |= S_NOCTTY;

  ret = sc_open(name,oflags,mode);
  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int open(const char *name,int flags, ...) __attribute__ ((weak, alias ("_open")));
