/**
 * @file common.h
 * @brief Common definitions for use in HAL (eg : assert)
 *
 * @section LICENSE
 *  Copyright (C) 2009-2024 Kalray
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
