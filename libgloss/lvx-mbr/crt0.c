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

#include <sys/reent.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

__thread struct _reent _impure_thread_data;

extern void  __libc_init_array (void);
extern void  __libc_fini_array (void);
extern void exit(int status);
extern void atexit(void (*function)(void));
extern void *memset(void *s, int c, size_t n);
extern int main (int argc, char **argv, char **env);

void
__start1(int argc, char **argv, char **envp)
{
  int res;

  /* Finish initialization of reent */
  _REENT_INIT_PTR(&_impure_thread_data);

  /* Run the main function */
  __libc_init_array ();
  atexit (__libc_fini_array);

  res = main (argc, argv, envp);

  exit(res);
  while(1);
}

__END_DECLS
