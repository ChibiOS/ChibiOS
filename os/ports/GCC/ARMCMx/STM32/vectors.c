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

/**
 * @file    STM32/vectors.c
 * @brief   Interrupt vectors for the STM32 family.
 *
 * @defgroup ARMCMx_STM32_VECTORS STM32 Interrupt Vectors
 * @ingroup ARMCMx
 * @details Interrupt vectors for the STM32 family.
 * @{
 */

#include "ch.h"

#if !defined(STM32F10X_LD) && !defined(STM32F10X_LD_VL) &&                  \
    !defined(STM32F10X_MD) && !defined(STM32F10X_MD_VL) &&                  \
    !defined(STM32F10X_HD) && !defined(STM32F10X_XL) &&                     \
    !defined(STM32F10X_CL)
#include "board.h"
#endif

#if !defined(__DOXYGEN__)
extern void __ram_end__(void);
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
#if defined(STM32F10X_MD_VL) || defined(STM32F10X_HD) || defined(STM32F10X_CL)
extern void VectorEC(void);
extern void VectorF0(void);
extern void VectorF4(void);
#endif
#if defined(STM32F10X_HD) || defined(STM32F10X_CL)
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
#endif
#if defined(STM32F10X_CL)
extern void Vector130(void);
extern void Vector134(void);
extern void Vector138(void);
extern void Vector13C(void);
extern void Vector140(void);
extern void Vector144(void);
extern void Vector148(void);
extern void Vector14C(void);
#endif
#endif

/**
 * @brief   STM32 vectors table.
 */
#if !defined(__DOXYGEN__)
__attribute__ ((section("vectors")))
#endif
void  (*_vectors[])(void) = {
  __ram_end__,
  ResetHandler,
  NMIVector,
  HardFaultVector,
  MemManageVector,
  BusFaultVector,
  UsageFaultVector,
  Vector1C,
  Vector20,
  Vector24,
  Vector28,
  SVCallVector,
  DebugMonitorVector,
  Vector34,
  PendSVVector,
  SysTickVector,
  Vector40,  Vector44,  Vector48,  Vector4C,
  Vector50,  Vector54,  Vector58,  Vector5C,
  Vector60,  Vector64,  Vector68,  Vector6C,
  Vector70,  Vector74,  Vector78,  Vector7C,
  Vector80,  Vector84,  Vector88,  Vector8C,
  Vector90,  Vector94,  Vector98,  Vector9C,
  VectorA0,  VectorA4,  VectorA8,  VectorAC,
  VectorB0,  VectorB4,  VectorB8,  VectorBC,
  VectorC0,  VectorC4,  VectorC8,  VectorCC,
  VectorD0,  VectorD4,  VectorD8,  VectorDC,
  VectorE0,  VectorE4,  VectorE8,
#if defined(STM32F10X_MD_VL) || defined(STM32F10X_HD) || defined(STM32F10X_CL)
  VectorEC, VectorF0, VectorF4,
#endif
#if defined(STM32F10X_HD) || defined(STM32F10X_CL)
  VectorF8,  VectorFC,  Vector100, Vector104,
  Vector108, Vector10C, Vector110, Vector114,
  Vector118, Vector11C, Vector120, Vector124,
  Vector128, Vector12C,
#endif
#if defined(STM32F10X_CL)
  Vector130, Vector134, Vector138, Vector13C,
  Vector140, Vector144, Vector148, Vector14C
#endif
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
    ".weak NMIVector                  \n"
    "NMIVector:                     \n\t"
    ".weak HardFaultVector            \n"
    "HardFaultVector:               \n\t"
    ".weak MemManageVector            \n"
    "MemManageVector:               \n\t"
    ".weak BusFaultVector             \n"
    "BusFaultVector:                \n\t"
    ".weak UsageFaultVector           \n"
    "UsageFaultVector:              \n\t"
    ".weak Vector1C                   \n"
    "Vector1C:                      \n\t"
    ".weak Vector20                   \n"
    "Vector20:                      \n\t"
    ".weak Vector24                   \n"
    "Vector24:                      \n\t"
    ".weak Vector28                   \n"
    "Vector28:                      \n\t"
    ".weak SVCallVector               \n"
    "SVCallVector:                  \n\t"
    ".weak DebugMonitorVector         \n"
    "DebugMonitorVector:            \n\t"
    ".weak Vector34                   \n"
    "Vector34:                      \n\t"
    ".weak PendSVVector               \n"
    "PendSVVector:                  \n\t"
    ".weak SysTickVector              \n"
    "SysTickVector:                 \n\t"
    ".weak Vector40                   \n"
    "Vector40:                      \n\t"
    ".weak Vector44                   \n"
    "Vector44:                      \n\t"
    ".weak Vector48                   \n"
    "Vector48:                      \n\t"
    ".weak Vector4C                   \n"
    "Vector4C:                      \n\t"
    ".weak Vector50                   \n"
    "Vector50:                      \n\t"
    ".weak Vector54                   \n"
    "Vector54:                      \n\t"
    ".weak Vector58                   \n"
    "Vector58:                      \n\t"
    ".weak Vector5C                   \n"
    "Vector5C:                      \n\t"
    ".weak Vector60                   \n"
    "Vector60:                      \n\t"
    ".weak Vector64                   \n"
    "Vector64:                      \n\t"
    ".weak Vector68                   \n"
    "Vector68:                      \n\t"
    ".weak Vector6C                   \n"
    "Vector6C:                      \n\t"
    ".weak Vector70                   \n"
    "Vector70:                      \n\t"
    ".weak Vector74                   \n"
    "Vector74:                      \n\t"
    ".weak Vector78                   \n"
    "Vector78:                      \n\t"
    ".weak Vector7C                   \n"
    "Vector7C:                      \n\t"
    ".weak Vector80                   \n"
    "Vector80:                      \n\t"
    ".weak Vector84                   \n"
    "Vector84:                      \n\t"
    ".weak Vector88                   \n"
    "Vector88:                      \n\t"
    ".weak Vector8C                   \n"
    "Vector8C:                      \n\t"
    ".weak Vector90                   \n"
    "Vector90:                      \n\t"
    ".weak Vector94                   \n"
    "Vector94:                      \n\t"
    ".weak Vector98                   \n"
    "Vector98:                      \n\t"
    ".weak Vector9C                   \n"
    "Vector9C:                      \n\t"
    ".weak VectorA0                   \n"
    "VectorA0:                      \n\t"
    ".weak VectorA4                   \n"
    "VectorA4:                      \n\t"
    ".weak VectorA8                   \n"
    "VectorA8:                      \n\t"
    ".weak VectorAC                   \n"
    "VectorAC:                      \n\t"
    ".weak VectorB0                   \n"
    "VectorB0:                      \n\t"
    ".weak VectorB4                   \n"
    "VectorB4:                      \n\t"
    ".weak VectorB8                   \n"
    "VectorB8:                      \n\t"
    ".weak VectorBC                   \n"
    "VectorBC:                      \n\t"
    ".weak VectorC0                   \n"
    "VectorC0:                      \n\t"
    ".weak VectorC4                   \n"
    "VectorC4:                      \n\t"
    ".weak VectorC8                   \n"
    "VectorC8:                      \n\t"
    ".weak VectorCC                   \n"
    "VectorCC:                      \n\t"
    ".weak VectorD0                   \n"
    "VectorD0:                      \n\t"
    ".weak VectorD4                   \n"
    "VectorD4:                      \n\t"
    ".weak VectorD8                   \n"
    "VectorD8:                      \n\t"
    ".weak VectorDC                   \n"
    "VectorDC:                      \n\t"
    ".weak VectorE0                   \n"
    "VectorE0:                      \n\t"
    ".weak VectorE4                   \n"
    "VectorE4:                      \n\t"
    ".weak VectorE8                   \n"
    "VectorE8:                      \n\t"
    ".weak VectorEC                   \n"
    "VectorEC:                      \n\t"
    ".weak VectorF0                   \n"
    "VectorF0:                      \n\t"
    ".weak VectorF4                   \n"
    "VectorF4:                      \n\t"
    ".weak VectorF8                   \n"
    "VectorF8:                      \n\t"
    ".weak VectorFC                   \n"
    "VectorFC:                      \n\t"
    ".weak Vector100                  \n"
    "Vector100:                     \n\t"
    ".weak Vector104                  \n"
    "Vector104:                     \n\t"
    ".weak Vector108                  \n"
    "Vector108:                     \n\t"
    ".weak Vector10C                  \n"
    "Vector10C:                     \n\t"
    ".weak Vector110                  \n"
    "Vector110:                     \n\t"
    ".weak Vector114                  \n"
    "Vector114:                     \n\t"
    ".weak Vector118                  \n"
    "Vector118:                     \n\t"
    ".weak Vector11C                  \n"
    "Vector11C:                     \n\t"
    ".weak Vector120                  \n"
    "Vector120:                     \n\t"
    ".weak Vector124                  \n"
    "Vector124:                     \n\t"
    ".weak Vector128                  \n"
    "Vector128:                     \n\t"
    ".weak Vector12C                  \n"
    "Vector12C:                     \n\t"
    ".weak Vector130                  \n"
    "Vector130:                     \n\t"
    ".weak Vector134                  \n"
    "Vector134:                     \n\t"
    ".weak Vector138                  \n"
    "Vector138:                     \n\t"
    ".weak Vector13C                  \n"
    "Vector13C:                     \n\t"
    ".weak Vector140                  \n"
    "Vector140:                     \n\t"
    ".weak Vector144                  \n"
    "Vector144:                     \n\t"
    ".weak Vector148                  \n"
    "Vector148:                     \n\t"
    ".weak Vector14C                  \n"
    "Vector14C:                     \n\t"
  );

  while (TRUE)
    ;
}
/** @} */
