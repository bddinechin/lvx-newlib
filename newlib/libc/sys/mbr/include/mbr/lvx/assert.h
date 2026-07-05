/**
 * Copyright (C) 2009-2024 Kalray SA.
 *
 * All rights reserved.
 */
#ifndef _LVX_ASSERT_H
#define _LVX_ASSERT_H

#ifndef __ASSEMBLER__

#include <mbr/lvx/common.h>
#include <mbr/lvx/scall_no.h>
#include <mbr/lvx/syscall.h>

#  ifndef __lvx_assert

#    ifdef NDEBUG
#      define __lvx_assert(expr) (__LVX_ASSERT_VOID_CAST(0))
#    else
#      ifdef __linux__
#        define __lvx_assert(expr) (__LVX_ASSERT_VOID_CAST(0))
#      else
#        define __lvx_assert(expr) ((expr) ? __LVX_ASSERT_VOID_CAST(0) : (void)(__lvx_syscall_with_args(__NR_iss_exit, 1, 0, 0, 0, 0, 0)))
#      endif    /* __linux__ */
#    endif      /* NDEBUG */

#  endif        /*__lvx_assert */

#endif          /* !__ASSEMBLER__ */

#endif /* _LVX_ASSERT_H */
