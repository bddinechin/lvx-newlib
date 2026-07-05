/*
 * Copyright (C) 2019-2026 Liesme Tech.
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
#ifndef _LVX_SYSCALL_H
#define _LVX_SYSCALL_H

#include <stdint.h>

/*
 * Syscalls.
 */

/**
 * \fn uint64_t __lvx_syscall(const __lvx_uint32_t syscall)
 * \brief Do syscall with no arguments
 * \param syscall Syscall number
 * \return Result of the syscall
 */
extern uint64_t __lvx_syscall(const uint32_t syscall_nb);

/**
 * \fn uint64_t __lvx_syscall_with_args(const uint64_t syscall, uint64_t sc_arg0,
 *                       uint64_t sc_arg1,uint64_t sc_arg2,
 *                       uint64_t sc_arg3, uint64_t sc_arg4,
 *                       uint64_t sc_arg5)
 * \brief Do syscall with 6 arguments
 * \param syscall Syscall number
 * \param sc_arg0 First syscall argument
 * \param sc_arg1 Second syscall argument
 * \param sc_arg2 Third syscall argument
 * \param sc_arg3 Fourth syscall argument
 * \param sc_arg4 Fifth syscall argument
 * \param sc_arg5 Sixth syscall argument
 * \return Result of the syscall
 */
extern uint64_t __lvx_syscall_with_args(const uint32_t syscall_nb, uint64_t sc_arg0, uint64_t sc_arg1,
			 uint64_t sc_arg2, uint64_t sc_arg3, uint64_t sc_arg4,
			 uint64_t sc_arg5);

#endif      /* ndef _LVX_SYSCALL_H */
