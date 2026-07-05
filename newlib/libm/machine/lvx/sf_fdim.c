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

/*
 * File copied from libm/common/sf_fdim.c
 * We fix the case where operandes are +-inf as it must return zero.
 */

#include "fdlibm.h"

#ifdef __STDC__
	float fdimf(float x, float y)
#else
	float fdimf(x,y)
	float x;
	float y;
#endif
{
  int c = __fpclassifyf(x);
  int d = __fpclassifyf(y);

  if (c == FP_NAN || d == FP_NAN) return x - y;
  if (x <= y) return 0.0;

  return x - y;
}

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double fdim(double x, double y)
#else
	double fdim(x,y)
	double x;
	double y;
#endif
{
  return (double) fdimf((float) x, (float) y);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
