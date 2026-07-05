/**
 * @file common.h
 * @brief Common definitions for use in HAL (eg : assert)
 *
 * @section LICENSE
 *  Copyright (C) 2019-2026 Liesme Tech.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 3, or (at your option) any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  Under Section 7 of GPL version 3, you are granted additional
 *  permissions described in the GCC Runtime Library Exception, version
 *  3.1, as published by the Free Software Foundation.
 *
 *  You should have received a copy of the GNU General Public License and
 *  a copy of the GCC Runtime Library Exception along with this program;
 *  see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 *  <http://www.gnu.org/licenses/>.
 */
#ifndef _LVX_HAL_COMMON_H
#define _LVX_HAL_COMMON_H

#ifndef __ASSEMBLER__
#endif /* !__ASSEMBLER__ */

#if !defined(__GNUC_PREREQ)
#if defined __GNUC__ && defined __GNUC_MINOR__
#define __GNUC_PREREQ(maj, min)						\
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

#define __LVX_MKSTRING(a) #a

#if defined(__cplusplus) && __GNUC_PREREQ(2, 95)
# define __LVX_ASSERT_VOID_CAST static_cast < void >
#else
# define __LVX_ASSERT_VOID_CAST (void)
#endif

#if  !defined(__clang__)
# define __optimize(__level) __attribute__ ((optimize(__level)))
# define __error(MSG) __attribute__((error(#MSG)))
#else
/* We should find a way to check at compile time rather
 * than link time with clang
 */
# define __error(MSG)
# define __optimize(__level)
#endif

/*
 * Compile time Check
 */
#define _LVX_CT_ASSERT(X, MSG)						\
	({								\
		extern int __error("Assertion failure: "#MSG) compile_time_check(); \
		((X)?0:compile_time_check());				\
	})

/*
 * Unsupported feature guard
 */
#define _LVX_UNSUPPORTED_FEATURE  __error("Unsupported feature")

#endif
