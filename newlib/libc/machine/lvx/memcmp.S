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
 #include "asm.h"

.cfi_sections .debug_frame
.text
.global memcmp
.proc memcmp
.align 16
memcmp:
   .cfi_startproc
   cb.deqz $r2? .Lreturn_early
   so -32[$r12] = $r28r29r30r31
   compd.geu $r3 = $r2, 192
   make $r6 = 0
   addd $r12 = $r12, -144
   ;;
   .cfi_def_cfa_offset 144
   so 80[$r12] = $r24r25r26r27
   cb.deqz $r3? .Lremaining_160
   ;;
   lo.u $r32r33r34r35 = 0[$r0]
   addd $r2 = $r2, -192
   ;;
   lo.u $r36r37r38r39 = 0[$r1]
   make $r4 = 0xaaaaaaaaaaaaaaab
   make $r6 = 1
   ;;
   lo.u $r40r41r42r43 = 32[$r0]
   muludt $r4r5 = $r2, $r4
   ;;
   lo.u $r44r45r46r47 = 32[$r1]
   ;;
   lo.u $r48r49r50r51 = 64[$r0]
   ;;
   lo.u $r52r53r54r55 = 64[$r1]
   ;;
   lo.u $r56r57r58r59 = 96[$r0]
   ;;
   lo.u $r60r61r62r63 = 96[$r1]
   ;;
   lo.u $r24r25r26r27 = 128[$r0]
   extfz $r7 = $r5, 32+7-1, 7
   ;;
   so 48[$r12] = $r20r21r22r23
   ;;
   lo.u $r20r21r22r23 = 128[$r1]
   ;;
   so 16[$r12] = $r16r17r18r19
   ;;
   lo.u $r16r17r18r19 = 160[$r0]
   ;;
   sq 0[$r12] = $r14r15
   ;;
   lq.u $r14r15 = 160[$r1]
   ;;
   cb.deqz $r7? .Lstreaming_remaining
   lq.u $r10r11 = 176[$r1]
   addd $r1 = $r1, 192
   addd $r0 = $r0, 192
   ;;
   loopdo $r7, .Lstreaming_kernel
   ;;
   compd.ne $r28 = $r32, $r36
   compd.ne $r29 = $r33, $r37
   compd.ne $r30 = $r34, $r38
   compd.ne $r31 = $r35, $r39
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r32
   copyd $r5 = $r36
   addd $r2 = $r2, -192
   ;;
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r33
   copyd $r5 = $r37
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r34
   copyd $r5 = $r38
   compd.ne $r28 = $r40, $r44
   compd.ne $r29 = $r41, $r45
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r35
   copyd $r5 = $r39
   compd.ne $r30 = $r42, $r46
   compd.ne $r31 = $r43, $r47
   ;;
   lo.u $r32r33r34r35 = 0[$r0]
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r40
   copyd $r5 = $r44
   ;;
   lo.u $r36r37r38r39= 0[$r1]
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r41
   copyd $r5 = $r45
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r42
   copyd $r5 = $r46
   compd.ne $r28 = $r48, $r52
   compd.ne $r29 = $r49, $r53
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r43
   copyd $r5 = $r47
   compd.ne $r30 = $r50, $r54
   compd.ne $r31 = $r51, $r55
   ;;
   lo.u $r40r41r42r43 = 32[$r0]
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r48
   copyd $r5 = $r52
   ;;
   lo.u $r44r45r46r47 = 32[$r1]
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r49
   copyd $r5 = $r53
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r50
   copyd $r5 = $r54
   compd.ne $r28 = $r56, $r60
   compd.ne $r29 = $r57, $r61
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r51
   copyd $r5 = $r55
   compd.ne $r30 = $r58, $r62
   compd.ne $r31 = $r59, $r63
   ;;
   lo.u $r48r49r50r51 = 64[$r0]
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r56
   copyd $r5 = $r60
   ;;
   lo.u $r52r53r54r55 = 64[$r1]
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r57
   copyd $r5 = $r61
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r58
   copyd $r5 = $r62
   compd.ne $r28 = $r24, $r20
   compd.ne $r29 = $r25, $r21
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r59
   copyd $r5 = $r63
   lo.u $r56r57r58r59= 96[$r0]
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r24
   copyd $r5 = $r20
   lo.u $r60r61r62r63= 96[$r1]
   ;;
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r25
   copyd $r5 = $r21
   compd.ne $r30 = $r26, $r22
   compd.ne $r31 = $r27, $r23
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r26
   copyd $r5 = $r22
   compd.ne $r28 = $r16, $r14
   compd.ne $r29 = $r17, $r15
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r27
   copyd $r5 = $r23
   lo.u $r24r25r26r27 = 128[$r0]
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r16
   copyd $r5 = $r14
   lo.u $r20r21r22r23 = 128[$r1]
   ;;
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r17
   copyd $r5 = $r15
   compd.ne $r30 = $r18, $r10
   compd.ne $r31 = $r19, $r11
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r18
   copyd $r5 = $r10
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r19
   copyd $r5 = $r11
   lo.u $r16r17r18r19 = 160[$r0]
   ;;
   lq.u $r14r15 = 160[$r1]
   ;;
   lq.u $r10r11 = 176[$r1]
   addd $r1 = $r1, 192
   addd $r0 = $r0, 192
   ;;
.Lstreaming_kernel:
.Lstreaming_remaining:
   compd.ne $r28 = $r32, $r36
   compd.ne $r29 = $r33, $r37
   compd.ne $r30 = $r34, $r38
   compd.ne $r31 = $r35, $r39
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r32
   copyd $r5 = $r36
   ;;
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r33
   copyd $r5 = $r37
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r34
   copyd $r5 = $r38
   compd.ne $r28 = $r40, $r44
   compd.ne $r29 = $r41, $r45
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r35
   copyd $r5 = $r39
   compd.ne $r30 = $r42, $r46
   compd.ne $r31 = $r43, $r47
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r40
   copyd $r5 = $r44
   ;;
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r41
   copyd $r5 = $r45
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r42
   copyd $r5 = $r46
   compd.ne $r28 = $r48, $r52
   compd.ne $r29 = $r49, $r53
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r43
   copyd $r5 = $r47
   compd.ne $r30 = $r50, $r54
   compd.ne $r31 = $r51, $r55
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r48
   copyd $r5 = $r52
   ;;
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r49
   copyd $r5 = $r53
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r50
   copyd $r5 = $r54
   compd.ne $r28 = $r56, $r60
   compd.ne $r29 = $r57, $r61
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r51
   copyd $r5 = $r55
   compd.ne $r30 = $r58, $r62
   compd.ne $r31 = $r59, $r63
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r56
   copyd $r5 = $r60
   ;;
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r57
   copyd $r5 = $r61
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r58
   copyd $r5 = $r62
   compd.ne $r28 = $r24, $r20
   compd.ne $r29 = $r25, $r21
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r59
   copyd $r5 = $r63
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r24
   copyd $r5 = $r20
   ;;
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r25
   copyd $r5 = $r21
   compd.ne $r30 = $r26, $r22
   compd.ne $r31 = $r27, $r23
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r26
   copyd $r5 = $r22
   compd.ne $r28 = $r16, $r14
   compd.ne $r29 = $r17, $r15
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r27
   copyd $r5 = $r23
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r16
   copyd $r5 = $r14
   ;;
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r17
   copyd $r5 = $r15
   compd.ne $r30 = $r18, $r10
   compd.ne $r31 = $r19, $r11
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r18
   copyd $r5 = $r10
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r19
   copyd $r5 = $r11
   ;;
.Lremaining_160:
   ldcond o, "", .dnez, $r6, $r24r25r26r27, 80[$r12]
   andd $r11 = $r2, 16
   srld $r7 = $r2, 5
   srld $r10 = $r2, 5
   ;;
   cb.deqz $r7? .Lloop_remaining_32
   ldcond o, "", .dnez, $r7, $r32r33r34r35, 0[$r0]
   ;;
   lo $r36r37r38r39 = 0[$r1]
   ;;
   loopdo $r7, .Lloop_kernel_256
   ;;
   compd.ne $r28 = $r32, $r36
   compd.ne $r29 = $r33, $r37
   compd.ne $r30 = $r34, $r38
   compd.ne $r31 = $r35, $r39
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r32
   copyd $r5 = $r36
   ;;
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r33
   copyd $r5 = $r37
   addd $r10 = $r10, -1
   ;;
   cb.dnez $r30? .Lstreaming_kernel_exit
   copyd $r4 = $r34
   copyd $r5 = $r38
   addd $r1 = $r1, 32
   addd $r0 = $r0, 32
   ;;
   cb.dnez $r31? .Lstreaming_kernel_exit
   copyd $r4 = $r35
   copyd $r5 = $r39
   ;;
   cb.deqz $r10? .Lloop_remaining_32
   ldcond o, "", .dnez, $r10, $r32r33r34r35, 0[$r0]
   ;;
   lo $r36r37r38r39 = 0[$r1]
   ;;
.Lloop_kernel_256:
.Lloop_remaining_32:
   andd $r10 = $r2, 8
   andd $r9 = $r2, 4
   cb.deqz $r11? .Lloop_remaining_16
   ldcond q, "", .dnez, $r11, $r32r33, 0[$r0]
   ;;
   lq $r36r37 = 0[$r1]
   addd $r1 = $r1, 16
   addd $r0 = $r0, 16
   ;;
   compd.ne $r28 = $r32, $r36
   compd.ne $r29 = $r33, $r37
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r32
   copyd $r5 = $r36
   ;;
   cb.dnez $r29? .Lstreaming_kernel_exit
   copyd $r4 = $r33
   copyd $r5 = $r37
   ;;
.Lloop_remaining_16:
   andd $r8 = $r2, 2
   andd $r7 = $r2, 1
   cb.deqz $r10? .Lloop_remaining_8
   ldcond d, "", .dnez, $r10, $r32, 0[$r0]
   ;;
   ld $r36 = 0[$r1]
   addd $r1 = $r1, 8
   addd $r0 = $r0, 8
   ;;
   compd.ne $r28 = $r32, $r36
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r32
   copyd $r5 = $r36
   ;;
.Lloop_remaining_8:
   cb.deqz $r9? .Lloop_remaining_4
   ldcond wz, "", .dnez, $r9, $r32, 0[$r0]
   ;;
   lwz $r36 = 0[$r1]
   addd $r1 = $r1, 4
   addd $r0 = $r0, 4
   ;;
   compd.ne $r28 = $r32, $r36
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r32
   copyd $r5 = $r36
   ;;
.Lloop_remaining_4:
   cb.deqz $r8? .Lloop_remaining_2
   ldcond hz, "", .dnez, $r8, $r32, 0[$r0]
   ;;
   lhz $r36 = 0[$r1]
   addd $r1 = $r1, 2
   addd $r0 = $r0, 2
   ;;
   compd.ne $r28 = $r32, $r36
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r32
   copyd $r5 = $r36
   ;;
.Lloop_remaining_2:
   cb.deqz $r7? .Lreturn
   ldcond bz, "", .dnez, $r7, $r32, 0[$r0]
   ;;
   lbz $r36 = 0[$r1]
   ;;
   compd.ne $r28 = $r32, $r36
   ;;
   cb.dnez $r28? .Lstreaming_kernel_exit
   copyd $r4 = $r32
   copyd $r5 = $r36
   ;;
.Lreturn:
   ldcond q, "", .dnez, $r6, $r14r15, 0[$r12]
   ;;
   ldcond o, "", .dnez, $r6, $r16r17r18r19, 16[$r12]
   ;;
   ldcond o, "", .dnez, $r6, $r20r21r22r23, 48[$r12]
   ;;
.Lreturn_early:
   addd $r12 = $r12, 144
   make $r0 = 0
   lo $r28r29r30r31 = 112[$r12]
   ret
   ;;
.Lstreaming_kernel_exit:
   eord $r0 = $r4, $r5
   ldcond q, "", .dnez, $r6, $r14r15, 0[$r12]
   ;;
   ctzd $r0 = $r0
   ldcond o, "", .dnez, $r6, $r16r17r18r19, 16[$r12]
   ;;
   andd $r0 = $r0, 0x38
   ldcond o, "", .dnez, $r6, $r20r21r22r23, 48[$r12]
   ;;
   srad $r4 = $r4, $r0
   srad $r5 = $r5, $r0
   ldcond o, "", .dnez, $r6, $r24r25r26r27, 80[$r12]
   ;;
   andd $r4 = $r4, 0xff
   andd $r5 = $r5, 0xff
   addd $r12 = $r12, 144
   lo $r28r29r30r31 = 112[$r12]
   ;;
  .cfi_def_cfa_offset 0
   sbfw $r0 = $r5, $r4
   ret
   ;;
  .cfi_endproc
.endp memcmp
.type name, @function
.size memcmp, . -memcmp
