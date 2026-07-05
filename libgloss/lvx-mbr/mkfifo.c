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

#include "sys/types.h"
#include "errno.h"

int sc_mkfifo(const char *__path, mode_t __mode);

int _mkfifo(const char *__path, mode_t __mode)
{
  int ret;

  ret = sc_mkfifo(__path,__mode);

  if(ret < 0) {
    errno = ret * -1;
    ret = -1;
  }
  return ret;
}

int mkfifo(const char *__path, mode_t __mode) __attribute__ ((weak, alias ("_mkfifo")));
