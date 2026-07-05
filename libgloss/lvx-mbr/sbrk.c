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

/* Implementation of non-OS memory allocation */

#include <unistd.h>
#include <errno.h>

extern void *_sbrk (ptrdiff_t);

extern char _heap_start ;
extern char _heap_end;
static char * _heap_ptr = NULL; /* calulated using symbols from the linker script: _heap_start and _heap_end */
static char *_end_of_allocatable_mem = NULL;

static int abs(int i)
{
  return (i < 0) ? -i : i;
}

void *_sbrk(ptrdiff_t nbytes) {
  /*  Arguments are:
   *  - number of bytes of memory to allocate
   *  This call atomically checks for overflow and increments the heap
   *  pointer if all is ok.
   *  The memory layout has the heap area starting above the user code and
   *  static data and growing up, and the stack starting in high memory
   *  (below kernel stack and environment, etc.) and growing down.
   */

  void * old_heap_ptr;
  void * returnCode;
  unsigned long initial_heap = 0;

  if (_heap_ptr == NULL) {
    unsigned long pagesz = getpagesize();

    /*
     * Align heap start on a page boundary like further allocations
     * in mallocr.c. This avoids differences in cache behavior due to
     * small increases of sections size.
     */
    _heap_ptr = (char *) ((((unsigned long) &_heap_start) + (pagesz - 1)) &
			  ~(pagesz - 1));
    initial_heap = (unsigned long) _heap_ptr;
    _end_of_allocatable_mem = &_heap_end;
  }
  old_heap_ptr = _heap_ptr ;

  /* Check overflows */
  if (nbytes >= 0) {
    if (! (nbytes <= ((unsigned long) _end_of_allocatable_mem -
		      (unsigned long) _heap_ptr))) {
      goto bad;
    }
  } else {
    if (! (abs(nbytes) <= ((unsigned long) old_heap_ptr - initial_heap))) {
      goto bad;
    }
  }

  _heap_ptr += nbytes;
  returnCode = old_heap_ptr;

  return returnCode;

 bad:
  errno = ENOMEM;
  return ((void *) -1);
}

void *sbrk(ptrdiff_t nbytes) __attribute__ ((weak, alias ("_sbrk")));
