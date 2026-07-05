/**
 * Copyright (C) 2009-2024 Kalray SA.
 *
 * All rights reserved.
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
