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
#include <sys/stat.h>
#include "errno.h"


int sc_stat(const char *filename, uint64_t *);

int _stat(const char *filename, struct stat *st)
{
  int ret;
  uint64_t res[13];

  ret = sc_stat(filename, res);

  if( ret < 0 ) {
    errno = ret * -1;
    ret = -1;
  } else {
    st->st_dev = res[0];
    st->st_ino = res[1];
    st->st_mode = res[2];
    st->st_nlink = res[3];
    st->st_uid = res[4];
    st->st_gid = res[5];
    st->st_rdev = res[6];
    st->st_size = res[7];
    st->st_blksize = res[8];
    st->st_blocks = res[9];
    st->st_atime = res[10];
    st->st_mtime = res[11];
    st->st_ctime = res[12];
  }

  return ret;
}

int stat(const char *filename, struct stat *st) __attribute__ ((weak, alias ("_stat")));
