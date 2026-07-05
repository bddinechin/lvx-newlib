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
