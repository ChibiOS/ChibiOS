/*
    ChibiOS - Copyright (C) 2006..2024 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    ARMv8-M/mpu.h
 * @brief   ARMv8-M MPU support macros and structures.
 *
 * @addtogroup ARMV8M_MPU
 * @{
 */

#ifndef MPU_V8M_H
#define MPU_V8M_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    MPU registers definitions
 * @{
 */
#define MPU_TYPE_SEPARATED                  (1U << 0)
#define MPU_TYPE_DREGION(n)                 (((n) >> 8) & 255U)

#define MPU_CTRL_ENABLE                     (1U << 0)
#define MPU_CTRL_HFNMIENA                   (1U << 1)
#define MPU_CTRL_PRIVDEFENA                 (1U << 2)

#define MPU_RNR_REGION_MASK                 (255U << 0)
#define MPU_RNR_REGION(n)                   ((n) << 0)

#define MPU_RBAR_XN                         (1U << 0)
#define MPU_RBAR_AP_MASK                    (3U << 1)
#define MPU_RBAR_AP(n)                      ((n) << 1)
#define MPU_RBAR_AP_RW_RO                   MPU_RBAR_AP(0U)
#define MPU_RBAR_AP_RW_RW                   MPU_RBAR_AP(1U)
#define MPU_RBAR_AP_RO_NA                   MPU_RBAR_AP(2U)
#define MPU_RBAR_AP_RO_RO                   MPU_RBAR_AP(3U)
#define MPU_RBAR_SH_MASK                    (3U << 3)
#define MPU_RBAR_SH(n)                      ((n) << 1)
#define MPU_RBAR_SH_NO                      MPU_RBAR_SH(0U)
#define MPU_RBAR_SH_OUTER                   MPU_RBAR_SH(1U)
#define MPU_RBAR_SH_INNER                   MPU_RBAR_SH(2U)
#define MPU_RBAR_BASE_MASK                  0xFFFFFFE0U
#define MPU_RBAR_BASE(n)                    ((n) << 5)

#define MPU_RLAR_ENABLE                     (1U << 0)
#define MPU_RLAR_ATTRINDX_MASK              (7U << 1)
#define MPU_RLAR_ATTRINDX(n)                ((n) << 1)
#define MPU_RLAR_LIMIT_MASK                 (0x0EFFFFFFU << 5)
#define MPU_RLAR_LIMIT(n)                   ((n) << 5)

#define MPU_MAIR0_ATTR0_MASK                (0xFFU << 0)
#define MPU_MAIR0_ATTR0(n)                  ((n) << 0)
#define MPU_MAIR0_ATTR1_MASK                (0xFFU << 8)
#define MPU_MAIR0_ATTR1(n)                  ((n) << 8)
#define MPU_MAIR0_ATTR2_MASK                (0xFFU << 16)
#define MPU_MAIR0_ATTR2(n)                  ((n) << 16)
#define MPU_MAIR0_ATTR3_MASK                (0xFFU << 24)
#define MPU_MAIR0_ATTR3(n)                  ((n) << 24)

#define MPU_MAIR1_ATTR4_MASK                (0xFFU << 0)
#define MPU_MAIR1_ATTR4(n)                  ((n) << 0)
#define MPU_MAIR1_ATTR5_MASK                (0xFFU << 8)
#define MPU_MAIR1_ATTR5(n)                  ((n) << 8)
#define MPU_MAIR1_ATTR6_MASK                (0xFFU << 16)
#define MPU_MAIR1_ATTR6(n)                  ((n) << 16)
#define MPU_MAIR1_ATTR7_MASK                (0xFFU << 24)
#define MPU_MAIR1_ATTR7(n)                  ((n) << 24)
/** @} */

/**
 * @name    Attributes in MAIR registers
 * @{
 */
#define MPU_MAIR_DEVICE_nGnRnE              0x00U
#define MPU_MAIR_DEVICE_nGnRE               0x04U
#define MPU_MAIR_DEVICE_nGRE                0x08U
#define MPU_MAIR_DEVICE_GRE                 0x0CU

#define MPU_MAIR_MEM_OUTER_TRANSIENT        0x00U
#define MPU_MAIR_MEM_OUTER_NON_TRANSIENT    0x80U
#define MPU_MAIR_MEM_OUTER_WRITE_THROUGH    0x00U
#define MPU_MAIR_MEM_OUTER_WRITE_BACK       0x40U
#define MPU_MAIR_MEM_OUTER_R_ALLOCATE       0x20U
#define MPU_MAIR_MEM_OUTER_W_ALLOCATE       0x10U
#define MPU_MAIR_MEM_OUTER_RW_ALLOCATE      0x30U

#define MPU_MAIR_MEM_INNER_TRANSIENT        0x00U
#define MPU_MAIR_MEM_INNER_NON_TRANSIENT    0x08U
#define MPU_MAIR_MEM_INNER_WRITE_THROUGH    0x00U
#define MPU_MAIR_MEM_INNER_WRITE_BACK       0x04U
#define MPU_MAIR_MEM_INNER_R_ALLOCATE       0x02U
#define MPU_MAIR_MEM_INNER_W_ALLOCATE       0x01U
#define MPU_MAIR_MEM_INNER_RW_ALLOCATE      0x03U

#define MPU_MAIR_MEM_OUTER_WT_T_WA          0x10U
#define MPU_MAIR_MEM_OUTER_WT_T_RA          0x20U
#define MPU_MAIR_MEM_OUTER_WT_T_RWA         0x30U
#define MPU_MAIR_MEM_OUTER_NC               0x40U
#define MPU_MAIR_MEM_OUTER_WB_T_WA          0x50U
#define MPU_MAIR_MEM_OUTER_WB_T_RA          0x60U
#define MPU_MAIR_MEM_OUTER_WB_T_RWA         0x70U
#define MPU_MAIR_MEM_OUTER_RESVD1           0x80U
#define MPU_MAIR_MEM_OUTER_WT_NT_WA         0x90U
#define MPU_MAIR_MEM_OUTER_WT_NT_RA         0xA0U
#define MPU_MAIR_MEM_OUTER_WT_NT_RWA        0xB0U
#define MPU_MAIR_MEM_OUTER_RESVD2           0xC0U
#define MPU_MAIR_MEM_OUTER_WB_NT_WA         0xD0U
#define MPU_MAIR_MEM_OUTER_WB_NT_RA         0xE0U
#define MPU_MAIR_MEM_OUTER_WB_NT_RWA        0xF0U

#define MPU_MAIR_MEM_INNER_WT_T_WA          0x01U
#define MPU_MAIR_MEM_INNER_WT_T_RA          0x02U
#define MPU_MAIR_MEM_INNER_WT_T_RWA         0x03U
#define MPU_MAIR_MEM_INNER_NC               0x04U
#define MPU_MAIR_MEM_INNER_WB_T_WA          0x05U
#define MPU_MAIR_MEM_INNER_WB_T_RA          0x06U
#define MPU_MAIR_MEM_INNER_WB_T_RWA         0x07U
#define MPU_MAIR_MEM_INNER_RESVD1           0x08U
#define MPU_MAIR_MEM_INNER_WT_NT_WA         0x09U
#define MPU_MAIR_MEM_INNER_WT_NT_RA         0x0AU
#define MPU_MAIR_MEM_INNER_WT_NT_RWA        0x0BU
#define MPU_MAIR_MEM_INNER_RESVD2           0x0CU
#define MPU_MAIR_MEM_INNER_WB_NT_WA         0x0DU
#define MPU_MAIR_MEM_INNER_WB_NT_RA         0x0EU
#define MPU_MAIR_MEM_INNER_WB_NT_RWA        0x0FU
/** @} */

/**
 * @name    Region identifiers
 * @{
 */
#define MPU_REGION_0                        0U
#define MPU_REGION_1                        1U
#define MPU_REGION_2                        2U
#define MPU_REGION_3                        3U
#define MPU_REGION_4                        4U
#define MPU_REGION_5                        5U
#define MPU_REGION_6                        6U
#define MPU_REGION_7                        7U
#define MPU_REGION_8                        7U
#define MPU_REGION_9                        7U
#define MPU_REGION_10                       7U
#define MPU_REGION_11                       7U
#define MPU_REGION_12                       7U
#define MPU_REGION_13                       7U
#define MPU_REGION_14                       7U
#define MPU_REGION_15                       7U
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Enables the MPU.
 * @note    MEMFAULENA is enabled in SCB_SHCSR.
 *
 * @param[in] ctrl      MPU control modes as defined in @p MPU_CTRL register,
 *                      the enable bit is enforced
 *
 * @api
 */
#define mpuEnable(ctrl) {                                                   \
  MPU->CTRL = ((uint32_t)ctrl) | MPU_CTRL_ENABLE;                           \
}

/**
 * @brief   Disables the MPU.
 * @note    MEMFAULENA is disabled in SCB_SHCSR.
 *
 * @api
 */
#define mpuDisable() {                                                      \
  SCB->SHCSR &= ~SCB_SHCSR_MEMFAULTENA_Msk;                                 \
}

/**
 * @brief   Configures an MPU region.
 *
 * @param[in] region    the region number
 * @param[in] rbar      RBAR register initialization value
 * @param[in] rlar      RAR register initialization value
 *
 * @api
 */
#define mpuConfigureRegion(region, rbar, rlar) {                            \
  MPU->RNR  = ((uint32_t)region);                                           \
  MPU->RBAR = ((uint32_t)rbar);                                             \
  MPU->RLAR = ((uint32_t)rlar);                                             \
}

/**
 * @brief   Changes an MPU region base address only.
 *
 * @param[in] region    the region number
 * @param[in] address   new start address of the region
 *
 * @api
 */
#define mpuSetRegionAddress(region, addr) {                                 \
  MPU->RNR  = ((uint32_t)region);                                           \
  MPU->RBAR = (MPU->RBAR & ~MPU_RBAR_BASE_MASK) |                           \
              ((uint32_t)addr & MPU_RBAR_BASE_MASK);                        \
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#endif /* MPU_V8M_H */

/** @} */
