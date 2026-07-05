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

	.section .text, "ax", @progbits
	.align 4

#define SYSCALL_ENTRY(name) \
	.proc sc_##name		; \
	.global sc_##name   	; \
sc_##name:


SYSCALL_ENTRY(access)
	scall __NR_access
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(chdir)
	scall __NR_chdir
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(chmod)
	scall __NR_chmod
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(close)
	scall __NR_close
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(dup2)
	scall __NR_dup2
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(dup)
	scall __NR_dup
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(exit)
	scall __NR_exit
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(fcntl)
	scall __NR_fcntl
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(fstat)
	scall __NR_fstat
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(gettimeofday)
	scall __NR_gettimeofday
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(isatty)
	scall __NR_isatty
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(link)
	scall __NR_link
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(lseek)
	scall __NR_lseek
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(mkdir)
	scall __NR_mkdir
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(mkfifo)
	scall __NR_iss_mkfifo
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(open)
	scall __NR_open
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(read)
	scall __NR_read
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(rmdir)
	scall __NR_rmdir
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(stat)
	scall __NR_stat
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(unlink)
	scall __NR_unlink
	;;
	ret
	;;
	.endp

SYSCALL_ENTRY(write)
	scall __NR_write
	;;
	ret
	;;
	.endp
