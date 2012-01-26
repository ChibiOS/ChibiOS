/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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

/**
 * @file    GCC/ARMCMx/STM32F4xx/vectors.c
 * @brief   Interrupt vectors for the STM32F4xx family.
 *
 * @defgroup ARMCMx_STM32F4xx_VECTORS STM32F4xx Interrupt Vectors
 * @ingroup ARMCMx_SPECIFIC
 * @details Interrupt vectors for the STM32F4xx family.
 * @{
 */

#include "ch.h"

#if !defined(__DOXYGEN__)
extern void __main_stack_end__(void);
extern void ResetHandler(void);
extern void NMIVector(void);
extern void HardFaultVector(void);
extern void MemManageVector(void);
extern void BusFaultVector(void);
extern void UsageFaultVector(void);
extern void Vector1C(void);
extern void Vector20(void);
extern void Vector24(void);
extern void Vector28(void);
extern void SVCallVector(void);
extern void DebugMonitorVector(void);
extern void Vector34(void);
extern void PendSVVector(void);
extern void SysTickVector(void);
extern void Vector40(void);
extern void Vector44(void);
extern void Vector48(void);
extern void Vector4C(void);
extern void Vector50(void);
extern void Vector54(void);
extern void Vector58(void);
extern void Vector5C(void);
extern void Vector60(void);
extern void Vector64(void);
extern void Vector68(void);
extern void Vector6C(void);
extern void Vector70(void);
extern void Vector74(void);
extern void Vector78(void);
extern void Vector7C(void);
extern void Vector80(void);
extern void Vector84(void);
extern void Vector88(void);
extern void Vector8C(void);
extern void Vector90(void);
extern void Vector94(void);
extern void Vector98(void);
extern void Vector9C(void);
extern void VectorA0(void);
extern void VectorA4(void);
extern void VectorA8(void);
extern void VectorAC(void);
extern void VectorB0(void);
extern void VectorB4(void);
extern void VectorB8(void);
extern void VectorBC(void);
extern void VectorC0(void);
extern void VectorC4(void);
extern void VectorC8(void);
extern void VectorCC(void);
extern void VectorD0(void);
extern void VectorD4(void);
extern void VectorD8(void);
extern void VectorDC(void);
extern void VectorE0(void);
extern void VectorE4(void);
extern void VectorE8(void);
extern void VectorEC(void);
extern void VectorF0(void);
extern void VectorF4(void);
extern void VectorF8(void);
extern void VectorFC(void);
extern void Vector100(void);
extern void Vector104(void);
extern void Vector108(void);
extern void Vector10C(void);
extern void Vector110(void);
extern void Vector114(void);
extern void Vector118(void);
extern void Vector11C(void);
extern void Vector120(void);
extern void Vector124(void);
extern void Vector128(void);
extern void Vector12C(void);
extern void Vector130(void);
extern void Vector134(void);
extern void Vector138(void);
extern void Vector13C(void);
extern void Vector140(void);
extern void Vector144(void);
extern void Vector148(void);
extern void Vector14C(void);
extern void Vector150(void);
extern void Vector154(void);
extern void Vector158(void);
extern void Vector15C(void);
extern void Vector160(void);
extern void Vector164(void);
extern void Vector168(void);
extern void Vector16C(void);
extern void Vector170(void);
extern void Vector174(void);
extern void Vector178(void);
extern void Vector17C(void);
extern void Vector180(void);
extern void Vector184(void);
#endif

/**
 * @brief   STM32 vectors table.
 */
#if !defined(__DOXYGEN__)
__attribute__ ((section("vectors")))
#endif
void  (*_vectors[])(void) = {
  __main_stack_end__, ResetHandler,       NMIVector,          HardFaultVector,
  MemManageVector,    BusFaultVector,     UsageFaultVector,   Vector1C,
  Vector20,           Vector24,           Vector28,           SVCallVector,
  DebugMonitorVector, Vector34,           PendSVVector,       SysTickVector,
  Vector40,           Vector44,           Vector48,           Vector4C,
  Vector50,           Vector54,           Vector58,           Vector5C,
  Vector60,           Vector64,           Vector68,           Vector6C,
  Vector70,           Vector74,           Vector78,           Vector7C,
  Vector80,           Vector84,           Vector88,           Vector8C,
  Vector90,           Vector94,           Vector98,           Vector9C,
  VectorA0,           VectorA4,           VectorA8,           VectorAC,
  VectorB0,           VectorB4,           VectorB8,           VectorBC,
  VectorC0,           VectorC4,           VectorC8,           VectorCC,
  VectorD0,           VectorD4,           VectorD8,           VectorDC,
  VectorE0,           VectorE4,           VectorE8,           VectorEC,
  VectorF0,           VectorF4,           VectorF8,           VectorFC,
  Vector100,          Vector104,          Vector108,          Vector10C,
  Vector110,          Vector114,          Vector118,          Vector11C,
  Vector120,          Vector124,          Vector128,          Vector12C,
  Vector130,          Vector134,          Vector138,          Vector13C,
  Vector140,          Vector144,          Vector148,          Vector14C,
  Vector150,          Vector154,          Vector158,          Vector15C,
  Vector160,          Vector164,          Vector168,          Vector16C,
  Vector170,          Vector174,          Vector178,          Vector17C,
  Vector180,          Vector184
};

/**
 * @brief   Unhandled exceptions handler.
 * @details Any undefined exception vector points to this function by default.
 *          This function simply stops the system into an infinite loop.
 *
 * @notapi
 */
#if !defined(__DOXYGEN__)
__attribute__ ((naked))
#endif
void _unhandled_exception(void) {

  asm volatile (
    ".weak NMIVector            \nNMIVector:                \n\t"
    ".weak HardFaultVector      \nHardFaultVector:          \n\t"
    ".weak MemManageVector      \nMemManageVector:          \n\t"
    ".weak BusFaultVector       \nBusFaultVector:           \n\t"
    ".weak UsageFaultVector     \nUsageFaultVector:         \n\t"
    ".weak Vector1C             \nVector1C:                 \n\t"
    ".weak Vector20             \nVector20:                 \n\t"
    ".weak Vector24             \nVector24:                 \n\t"
    ".weak Vector28             \nVector28:                 \n\t"
    ".weak SVCallVector         \nSVCallVector:             \n\t"
    ".weak DebugMonitorVector   \nDebugMonitorVector:       \n\t"
    ".weak Vector34             \nVector34:                 \n\t"
    ".weak PendSVVector         \nPendSVVector:             \n\t"
    ".weak SysTickVector        \nSysTickVector:            \n\t"
    ".weak Vector40             \nVector40:                 \n\t"
    ".weak Vector44             \nVector44:                 \n\t"
    ".weak Vector48             \nVector48:                 \n\t"
    ".weak Vector4C             \nVector4C:                 \n\t"
    ".weak Vector50             \nVector50:                 \n\t"
    ".weak Vector54             \nVector54:                 \n\t"
    ".weak Vector58             \nVector58:                 \n\t"
    ".weak Vector5C             \nVector5C:                 \n\t"
    ".weak Vector60             \nVector60:                 \n\t"
    ".weak Vector64             \nVector64:                 \n\t"
    ".weak Vector68             \nVector68:                 \n\t"
    ".weak Vector6C             \nVector6C:                 \n\t"
    ".weak Vector70             \nVector70:                 \n\t"
    ".weak Vector74             \nVector74:                 \n\t"
    ".weak Vector78             \nVector78:                 \n\t"
    ".weak Vector7C             \nVector7C:                 \n\t"
    ".weak Vector80             \nVector80:                 \n\t"
    ".weak Vector84             \nVector84:                 \n\t"
    ".weak Vector88             \nVector88:                 \n\t"
    ".weak Vector8C             \nVector8C:                 \n\t"
    ".weak Vector90             \nVector90:                 \n\t"
    ".weak Vector94             \nVector94:                 \n\t"
    ".weak Vector98             \nVector98:                 \n\t"
    ".weak Vector9C             \nVector9C:                 \n\t"
    ".weak VectorA0             \nVectorA0:                 \n\t"
    ".weak VectorA4             \nVectorA4:                 \n\t"
    ".weak VectorA8             \nVectorA8:                 \n\t"
    ".weak VectorAC             \nVectorAC:                 \n\t"
    ".weak VectorB0             \nVectorB0:                 \n\t"
    ".weak VectorB4             \nVectorB4:                 \n\t"
    ".weak VectorB8             \nVectorB8:                 \n\t"
    ".weak VectorBC             \nVectorBC:                 \n\t"
    ".weak VectorC0             \nVectorC0:                 \n\t"
    ".weak VectorC4             \nVectorC4:                 \n\t"
    ".weak VectorC8             \nVectorC8:                 \n\t"
    ".weak VectorCC             \nVectorCC:                 \n\t"
    ".weak VectorD0             \nVectorD0:                 \n\t"
    ".weak VectorD4             \nVectorD4:                 \n\t"
    ".weak VectorD8             \nVectorD8:                 \n\t"
    ".weak VectorDC             \nVectorDC:                 \n\t"
    ".weak VectorE0             \nVectorE0:                 \n\t"
    ".weak VectorE4             \nVectorE4:                 \n\t"
    ".weak VectorE8             \nVectorE8:                 \n\t"
    ".weak VectorEC             \nVectorEC:                 \n\t"
    ".weak VectorF0             \nVectorF0:                 \n\t"
    ".weak VectorF4             \nVectorF4:                 \n\t"
    ".weak VectorF8             \nVectorF8:                 \n\t"
    ".weak VectorFC             \nVectorFC:                 \n\t"
    ".weak Vector100            \nVector100:                \n\t"
    ".weak Vector104            \nVector104:                \n\t"
    ".weak Vector108            \nVector108:                \n\t"
    ".weak Vector10C            \nVector10C:                \n\t"
    ".weak Vector110            \nVector110:                \n\t"
    ".weak Vector114            \nVector114:                \n\t"
    ".weak Vector118            \nVector118:                \n\t"
    ".weak Vector11C            \nVector11C:                \n\t"
    ".weak Vector120            \nVector120:                \n\t"
    ".weak Vector124            \nVector124:                \n\t"
    ".weak Vector128            \nVector128:                \n\t"
    ".weak Vector12C            \nVector12C:                \n\t"
    ".weak Vector130            \nVector130:                \n\t"
    ".weak Vector134            \nVector134:                \n\t"
    ".weak Vector138            \nVector138:                \n\t"
    ".weak Vector13C            \nVector13C:                \n\t"
    ".weak Vector140            \nVector140:                \n\t"
    ".weak Vector144            \nVector144:                \n\t"
    ".weak Vector148            \nVector148:                \n\t"
    ".weak Vector14C            \nVector14C:                \n\t"
    ".weak Vector150            \nVector150:                \n\t"
    ".weak Vector154            \nVector154:                \n\t"
    ".weak Vector158            \nVector158:                \n\t"
    ".weak Vector15C            \nVector15C:                \n\t"
    ".weak Vector160            \nVector160:                \n\t"
    ".weak Vector164            \nVector164:                \n\t"
    ".weak Vector168            \nVector168:                \n\t"
    ".weak Vector16C            \nVector16C:                \n\t"
    ".weak Vector170            \nVector170:                \n\t"
    ".weak Vector174            \nVector174:                \n\t"
    ".weak Vector178            \nVector178:                \n\t"
    ".weak Vector17C            \nVector17C:                \n\t"
    ".weak Vector180            \nVector180:                \n\t"
    ".weak Vector184            \nVector184:                \n\t"
  );

  while (TRUE)
    ;
}

/** @} */
