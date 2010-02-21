/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

.syntax unified
.thumb

.section vectors
_vectors:
        .word   __ram_end__
        .word   ResetHandler
        .word   NMIVector
        .word   HardFaultVector
        .word   MemManageVector
        .word   BusFaultVector
        .word   UsageFaultVector
        .word   Vector1C
        .word   Vector20
        .word   Vector24
        .word   Vector28
        .word   SVCallVector
        .word   DebugMonitorVector
        .word   Vector34
        .word   PendSVVector
        .word   SysTickVector
        .word   Vector40
        .word   Vector44
        .word   Vector48
        .word   Vector4C
        .word   Vector50
        .word   Vector54
        .word   Vector58
        .word   Vector5C
        .word   Vector60
        .word   Vector64
        .word   Vector68
        .word   Vector6C
        .word   Vector70
        .word   Vector74
        .word   Vector78
        .word   Vector7C
        .word   Vector80
        .word   Vector84
        .word   Vector88
        .word   Vector8C
        .word   Vector90
        .word   Vector94
        .word   Vector98
        .word   Vector9C
        .word   VectorA0
        .word   VectorA4
        .word   VectorA8
        .word   VectorAC
        .word   VectorB0
        .word   VectorB4
        .word   VectorB8
        .word   VectorBC
        .word   VectorC0
        .word   VectorC4
        .word   VectorC8
        .word   VectorCC
        .word   VectorD0
        .word   VectorD4
        .word   VectorD8
        .word   VectorDC
        .word   VectorE0
        .word   VectorE4
        .word   VectorE8
#if defined(STM32F10X_HD) || defined(STM32F10X_CL)
        .word   VectorEC
        .word   VectorF0
        .word   VectorF4
        .word   VectorF8
        .word   VectorFC
        .word   Vector100
        .word   Vector104
        .word   Vector108
        .word   Vector10C
        .word   Vector110
        .word   Vector114
        .word   Vector118
        .word   Vector11C
        .word   Vector120
        .word   Vector124
        .word   Vector128
        .word   Vector12C
#endif
#if defined(STM32F10X_CL)
        .word   Vector130
        .word   Vector134
        .word   Vector138
        .word   Vector13C
        .word   Vector140
        .word   Vector144
        .word   Vector148
        .word   Vector14C
#endif

.weak NMIVector
NMIVector:

.weak HardFaultVector
HardFaultVector:

.weak MemManageVector
MemManageVector:

.weak BusFaultVector
BusFaultVector:

.weak UsageFaultVector
UsageFaultVector:

.weak Vector1C
Vector1C:

.weak Vector20
Vector20:

.weak Vector24
Vector24:

.weak Vector28
Vector28:

.weak SVCallVector
SVCallVector:

.weak DebugMonitorVector
DebugMonitorVector:

.weak Vector34
Vector34:

.weak PendSVVector
PendSVVector:

.weak SysTickVector
SysTickVector:

.weak Vector40
Vector40:

.weak Vector44
Vector44:

.weak Vector48
Vector48:

.weak Vector4C
Vector4C:

.weak Vector50
Vector50:

.weak Vector54
Vector54:

.weak Vector58
Vector58:

.weak Vector5C
Vector5C:

.weak Vector60
Vector60:

.weak Vector64
Vector64:

.weak Vector68
Vector68:

.weak Vector6C
Vector6C:

.weak Vector70
Vector70:

.weak Vector74
Vector74:

.weak Vector78
Vector78:

.weak Vector7C
Vector7C:

.weak Vector80
Vector80:

.weak Vector84
Vector84:

.weak Vector88
Vector88:

.weak Vector8C
Vector8C:

.weak Vector90
Vector90:

.weak Vector94
Vector94:

.weak Vector98
Vector98:

.weak Vector9C
Vector9C:

.weak VectorA0
VectorA0:

.weak VectorA4
VectorA4:

.weak VectorA8
VectorA8:

.weak VectorAC
VectorAC:

.weak VectorB0
VectorB0:

.weak VectorB4
VectorB4:

.weak VectorB8
VectorB8:

.weak VectorBC
VectorBC:

.weak VectorC0
VectorC0:

.weak VectorC4
VectorC4:

.weak VectorC8
VectorC8:

.weak VectorCC
VectorCC:

.weak VectorD0
VectorD0:

.weak VectorD4
VectorD4:

.weak VectorD8
VectorD8:

.weak VectorDC
VectorDC:

.weak VectorE0
VectorE0:

.weak VectorE4
VectorE4:

.weak VectorE8
VectorE8:

#if defined(STM32F10X_HD) || defined(STM32F10X_CL)
.weak VectorEC
VectorEC:

.weak VectorF0
VectorF0:

.weak VectorF4
VectorF4:

.weak VectorF8
VectorF8:

.weak VectorFC
VectorFC:

.weak Vector100
Vector100:

.weak Vector104
Vector104:

.weak Vector108
Vector108:

.weak Vector10C
Vector10C:

.weak Vector110
Vector110:

.weak Vector114
Vector114:

.weak Vector118
Vector118:

.weak Vector11C
Vector11C:

.weak Vector120
Vector120:

.weak Vector124
Vector124:

.weak Vector128
Vector128:

.weak Vector12C
Vector12C:
#endif
#if defined(STM32F10X_CL)
.weak Vector130
Vector130:

.weak Vector134
Vector134:

.weak Vector138
Vector138:

.weak Vector13C
Vector13C:

.weak Vector140
Vector140:

.weak Vector144
Vector144:

.weak Vector148
Vector148:

.weak Vector14C
Vector14C:
#endif

here:   b       here
