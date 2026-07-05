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
 *
 * NOTE: there is no LVX ISS yet, and no independently-defined LVX syscall
 * ABI. These numbers (and the S_* open()/fcntl() flag values) are reused
 * verbatim from KVX's scall_no.h by deliberate decision, matching how the
 * LVX ABI is otherwise kept identical to KVX's kv4-v1 ABI. Whoever builds
 * an LVX ISS should implement this same syscall interface; if a different
 * numbering is chosen later, update this file (and asm_syscalls.S, which
 * references these __NR_* names directly).
 */
#ifndef _LVX_SCALL_NO_H
#define _LVX_SCALL_NO_H

#ifdef __linux__
#  ifdef __KERNEL__
#    define _LVX_SCALL_NO_H_EMPTY
#  endif
#endif

#ifndef _LVX_SCALL_NO_H_EMPTY

#  ifndef _LVX_SCALL_NO_NATIF

/* 'open'/'fcntl' flags that will be passed between newlib/host */
#    define S_RDONLY    0x001
#    define S_WRONLY    0x002
#    define S_RDWR      0x004
#    define S_APPEND    0x008
#    define S_CREAT     0x010
#    define S_TRUNC     0x020
#    define S_EXCL      0x040
#    define S_SYNC      0x080
#    define S_NDELAY    0x100
#    define S_NONBLOCK  0x200
#    define S_NOCTTY    0x400

/* Those are syscall numbers for the ISS.
 *    TODO: split this into OS-dependant parts, with different compilations
 *          of newlib for each of them
 */
#    define __NR_exit           1
#    define __NR_close          4
#    define __NR_fstat          6
#    define __NR_link           7
#    define __NR_unlink         8
#    define __NR_lseek          9
#    define __NR_open           10
#    define __NR_read           11
#    define __NR_stat           14
#    define __NR_gettimeofday   16
#    define __NR_write          17
#    define __NR_chmod          18
#    define __NR_isatty         19
#    define __NR_getuid         24
#    define __NR_dup            28
#    define __NR_dup2           29
#    define __NR_mkdir          39
#    define __NR_rmdir          40
#    define __NR_getgid         47
#    define __NR_fcntl          48
#    define __NR_geteuid        49
#    define __NR_getegid        50
#    define __NR_access         52
#    define __NR_chdir          54
#    define __NR_mkfifo         55
#    define __NR_lstat          107
#    define __NR_getdents       141
#    define __NR_cache_flush    196
#    define __NR_exit_thread    197
#    define __NR_send_command   198
#    define __NR_readtimer      200
#define  __NR_probe_address_space 201

#    define __NR_last_syscall 202 /* Set to highest syscall number */


/* Special syscalls for internal use by the ISS */
#    define __NR_reset_cluster_cpus 0xfc9 /* 4041 */
#    define __NR_iss_gen_coredump   0xfca /* 4042 */
#    define __NR_iss_trace_en_set   0xfcb /* 4043 */
#    define __NR_iss_sim_mode_set   0xfcc /* 4044 */
#    define __NR_iss_lstat          0xfcd /* 4045 */
#    define __NR_iss_getuid         0xfce /* 4046 */
#    define __NR_iss_geteuid        0xfcf /* 4047 */
#    define __NR_iss_getgid         0xfd0 /* 4048 */
#    define __NR_iss_getegid        0xfd1 /* 4049 */
#    define __NR_BREAKPOINT_PL0     0xfd2 /* 4050 */
#    define __NR_BREAKPOINT_PL1     0xfd3 /* 4051 */
#    define __NR_BREAKPOINT_PL2     0xfd4 /* 4052 */
#    define __NR_BREAKPOINT_PL3     0xfd5 /* 4053 */
#    define __NR_BREAKPOINT_JTAGISS 0xfd6 /* 4054 */
#    define __NR_iss_trace_notify   0xfd7 /* 4055 */
#    define __NR_iss_is_simu        0xfd8 /* 4056 */
#    define __NR_iss_notify_spawn   0xfd9 /* 4057 */
#    define __NR_iss_inbyte         0xfda /* 4058 */
#    define __NR_iss_poll           0xfdb /* 4059 */
#    define __NR_iss_close          0xfde /* 4062 */
#    define __NR_iss_fstat          0xfdf /* 4063 */
#    define __NR_iss_load_elf       0xfe8 /* 4072 */
#    define __NR_iss_mkfifo         0xfe9 /* 4073 */
#    define __NR_iss_link           0xfea /* 4074 */
#    define __NR_iss_unlink         0xfeb /* 4075 */
#    define __NR_iss_lseek          0xfec /* 4076 */
#    define __NR_iss_open           0xfed /* 4077 */
#    define __NR_iss_read           0xfee /* 4078 */
#    define __NR_iss_stat           0xfef /* 4079 */
#    define __NR_iss_gettimeofday   0xff0 /* 4080 */
#    define __NR_iss_write          0xff1 /* 4081 */
#    define __NR_iss_chmod          0xff2 /* 4082 */
#    define __NR_iss_isatty         0xff3 /* 4083 */
#    define __NR_iss_dup            0xff4 /* 4084 */
#    define __NR_iss_dup2           0xff5 /* 4085 */
#    define __NR_iss_fcntl          0xff6 /* 4086 */
#    define __NR_iss_mkdir          0xff7 /* 4087 */
#    define __NR_iss_rmdir          0xff8 /* 4088 */
#    define __NR_iss_access         0xff9 /* 4089 */
#    define __NR_iss_chdir          0xffa /* 4090 */
#    define __NR_iss_getdents       0xffb /* 4091 */
#    define __NR_iss_readtimer      0xffd /* 4093 */
#    define __NR_iss_printf         0xffe /* 4094 */
#    define __NR_iss_exit           0xfff /* 4095 */

#  endif    /* _LVX_SCALL_NO_NATIF */

#endif      /*_LVX_SCALL_NO_H_EMPTY */

#endif      /* ndef _LVX_SCALL_NO_H */
