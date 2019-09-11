/*
    ChibiOS - Copyright (C) 2006..2019 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    sb/sbhost.h
 * @brief   ARMv7-M sandbox macros and structures.
 *
 * @addtogroup ARMV7M_SANDBOX
 * @{
 */

#ifndef SBHOST_H
#define SBHOST_H

#include "sbapi.h"
#if defined(SB_INCLUDE_USERAPI)
#include "sbuserapi.h"
#endif

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Magic numbers
 * @{
 */
#define SB_MAGIC1                           0xFE9154C0U
#define SB_MAGIC2                           0x0C4519EFU
/** @} */

/**
 * @brief   Sandbox error codes
 * @{
 */
#define SB_ERR_NOERROR                      0U
#define SB_ERR_NOT_IMPLEMENTED              0xFFFFFFFFU
#define SB_ERR_MEMORY_FAULT                 0xFFFFFFFEU
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if PORT_USE_SYSCALL == FALSE
#error "sandbox requires PORT_USE_SYSCALL"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a syscall handler.
 */
typedef uint32_t (*port_syscall_t)(struct port_extctx *ctx);

/**
 * @brief   Type of a sandbox memory region.
 */
typedef struct {
  /**
   * @brief   Region 0 start address.
   */
  uint32_t                      r0_base;
  /**
   * @brief   Region 0 size.
   */
  uint32_t                      r0_end;
  /**
   * @brief   Region 2 start address.
   * @note    Zero if not used.
   */
  uint32_t                      r1_base;
  /**
   * @brief   Region 2 end address.
   * @note    Zero if not used.
   */
  uint32_t                      r1_end;
} sb_config_t;

/**
 * @brief   Type of a sandbox object.
 */
typedef struct {
  /**
   * @brief   Pointer to the sandbox configuration data.
   */
  const sb_config_t             *config;
  /**
   * @brief   Thread running in the sandbox.
   */
  thread_t                      *tp;
} sb_class_t;

/**
 * @brief   Type of a sandbox applet headers.
 */
typedef struct {
  /**
   * @brief   Magic number 1.
   */
  uint32_t                      hdr_magic1;
  /**
   * @brief   Magic number 2.
   */
  uint32_t                      hdr_magic2;
  /**
   * @brief   Header size, inclusive of magic numbers.
   */
  uint32_t                      hdr_size;
  /**
   * @brief   Used-defined parameters, defaulted to zero.
   */
  uint32_t                      user;
  /**
   * @brief   Region 0 start address.
   */
  uint32_t                      r0_base;
  /**
   * @brief   Region 0 size.
   */
  uint32_t                      r0_end;
  /**
   * @brief   Region 2 start address.
   * @note    Zero if not used.
   */
  uint32_t                      r1_base;
  /**
   * @brief   Region 2 end address.
   * @note    Zero if not used.
   */
  uint32_t                      r1_end;
} sb_header_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

#define __SVC(x) asm volatile ("svc " #x)

/*
 * All handlers defaulted to a common function.
 */
#if !defined(SB_SVC0_HANDLER)
#define SB_SVC0_HANDLER         sb_undef_handler
#endif
#if !defined(SB_SVC1_HANDLER)
#define SB_SVC1_HANDLER         sb_undef_handler
#endif
#if !defined(SB_SVC2_HANDLER)
#define SB_SVC2_HANDLER         sb_undef_handler
#endif
#if !defined(SB_SVC3_HANDLER)
#define SB_SVC3_HANDLER         sb_undef_handler
#endif
#if !defined(SB_SVC4_HANDLER)
#define SB_SVC4_HANDLER         sb_undef_handler
#endif
#if !defined(SB_SVC5_HANDLER)
#define SB_SVC5_HANDLER         sb_undef_handler
#endif
#if !defined(SB_SVC6_HANDLER)
#define SB_SVC6_HANDLER         sb_undef_handler
#endif
#if !defined(SB_SVC7_HANDLER)
#define SB_SVC7_HANDLER         sb_undef_handler
#endif
#if !defined(SB_SVC8_HANDLER)
#define SB_SVC8_HANDLER         sb_undef_handler
#endif
#if !defined(SB_SVC9_HANDLER)
#define SB_SVC9_HANDLER         sb_undef_handler
#endif
#if !defined(SB_SVC10_HANDLER)
#define SB_SVC10_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC11_HANDLER)
#define SB_SVC11_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC12_HANDLER)
#define SB_SVC12_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC13_HANDLER)
#define SB_SVC13_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC14_HANDLER)
#define SB_SVC14_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC15_HANDLER)
#define SB_SVC15_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC16_HANDLER)
#define SB_SVC16_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC17_HANDLER)
#define SB_SVC17_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC18_HANDLER)
#define SB_SVC18_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC19_HANDLER)
#define SB_SVC19_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC20_HANDLER)
#define SB_SVC20_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC21_HANDLER)
#define SB_SVC21_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC22_HANDLER)
#define SB_SVC22_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC23_HANDLER)
#define SB_SVC23_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC24_HANDLER)
#define SB_SVC24_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC25_HANDLER)
#define SB_SVC25_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC26_HANDLER)
#define SB_SVC26_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC27_HANDLER)
#define SB_SVC27_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC28_HANDLER)
#define SB_SVC28_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC29_HANDLER)
#define SB_SVC29_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC30_HANDLER)
#define SB_SVC30_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC31_HANDLER)
#define SB_SVC31_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC32_HANDLER)
#define SB_SVC32_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC33_HANDLER)
#define SB_SVC33_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC34_HANDLER)
#define SB_SVC34_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC35_HANDLER)
#define SB_SVC35_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC36_HANDLER)
#define SB_SVC36_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC37_HANDLER)
#define SB_SVC37_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC38_HANDLER)
#define SB_SVC38_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC39_HANDLER)
#define SB_SVC39_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC40_HANDLER)
#define SB_SVC40_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC41_HANDLER)
#define SB_SVC41_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC42_HANDLER)
#define SB_SVC42_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC43_HANDLER)
#define SB_SVC43_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC44_HANDLER)
#define SB_SVC44_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC45_HANDLER)
#define SB_SVC45_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC46_HANDLER)
#define SB_SVC46_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC47_HANDLER)
#define SB_SVC47_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC48_HANDLER)
#define SB_SVC48_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC49_HANDLER)
#define SB_SVC49_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC50_HANDLER)
#define SB_SVC50_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC51_HANDLER)
#define SB_SVC51_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC52_HANDLER)
#define SB_SVC52_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC53_HANDLER)
#define SB_SVC53_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC54_HANDLER)
#define SB_SVC54_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC55_HANDLER)
#define SB_SVC55_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC56_HANDLER)
#define SB_SVC56_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC57_HANDLER)
#define SB_SVC57_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC58_HANDLER)
#define SB_SVC58_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC59_HANDLER)
#define SB_SVC59_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC60_HANDLER)
#define SB_SVC60_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC61_HANDLER)
#define SB_SVC61_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC62_HANDLER)
#define SB_SVC62_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC63_HANDLER)
#define SB_SVC63_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC64_HANDLER)
#define SB_SVC64_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC65_HANDLER)
#define SB_SVC65_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC66_HANDLER)
#define SB_SVC66_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC67_HANDLER)
#define SB_SVC67_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC68_HANDLER)
#define SB_SVC68_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC69_HANDLER)
#define SB_SVC69_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC70_HANDLER)
#define SB_SVC70_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC71_HANDLER)
#define SB_SVC71_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC72_HANDLER)
#define SB_SVC72_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC73_HANDLER)
#define SB_SVC73_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC74_HANDLER)
#define SB_SVC74_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC75_HANDLER)
#define SB_SVC75_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC76_HANDLER)
#define SB_SVC76_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC77_HANDLER)
#define SB_SVC77_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC78_HANDLER)
#define SB_SVC78_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC79_HANDLER)
#define SB_SVC79_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC80_HANDLER)
#define SB_SVC80_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC81_HANDLER)
#define SB_SVC81_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC82_HANDLER)
#define SB_SVC82_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC83_HANDLER)
#define SB_SVC83_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC84_HANDLER)
#define SB_SVC84_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC85_HANDLER)
#define SB_SVC85_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC86_HANDLER)
#define SB_SVC86_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC87_HANDLER)
#define SB_SVC87_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC88_HANDLER)
#define SB_SVC88_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC89_HANDLER)
#define SB_SVC89_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC90_HANDLER)
#define SB_SVC90_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC91_HANDLER)
#define SB_SVC91_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC92_HANDLER)
#define SB_SVC92_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC93_HANDLER)
#define SB_SVC93_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC94_HANDLER)
#define SB_SVC94_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC95_HANDLER)
#define SB_SVC95_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC96_HANDLER)
#define SB_SVC96_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC97_HANDLER)
#define SB_SVC97_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC98_HANDLER)
#define SB_SVC98_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC99_HANDLER)
#define SB_SVC99_HANDLER        sb_undef_handler
#endif
#if !defined(SB_SVC100_HANDLER)
#define SB_SVC100_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC101_HANDLER)
#define SB_SVC101_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC102_HANDLER)
#define SB_SVC102_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC103_HANDLER)
#define SB_SVC103_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC104_HANDLER)
#define SB_SVC104_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC105_HANDLER)
#define SB_SVC105_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC106_HANDLER)
#define SB_SVC106_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC107_HANDLER)
#define SB_SVC107_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC108_HANDLER)
#define SB_SVC108_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC109_HANDLER)
#define SB_SVC109_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC110_HANDLER)
#define SB_SVC110_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC111_HANDLER)
#define SB_SVC111_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC112_HANDLER)
#define SB_SVC112_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC113_HANDLER)
#define SB_SVC113_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC114_HANDLER)
#define SB_SVC114_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC115_HANDLER)
#define SB_SVC115_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC116_HANDLER)
#define SB_SVC116_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC117_HANDLER)
#define SB_SVC117_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC118_HANDLER)
#define SB_SVC118_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC119_HANDLER)
#define SB_SVC119_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC120_HANDLER)
#define SB_SVC120_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC121_HANDLER)
#define SB_SVC121_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC122_HANDLER)
#define SB_SVC122_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC123_HANDLER)
#define SB_SVC123_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC124_HANDLER)
#define SB_SVC124_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC125_HANDLER)
#define SB_SVC125_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC126_HANDLER)
#define SB_SVC126_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC127_HANDLER)
#define SB_SVC127_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC128_HANDLER)
#define SB_SVC128_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC129_HANDLER)
#define SB_SVC129_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC130_HANDLER)
#define SB_SVC130_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC131_HANDLER)
#define SB_SVC131_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC132_HANDLER)
#define SB_SVC132_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC133_HANDLER)
#define SB_SVC133_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC134_HANDLER)
#define SB_SVC134_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC135_HANDLER)
#define SB_SVC135_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC136_HANDLER)
#define SB_SVC136_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC137_HANDLER)
#define SB_SVC137_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC138_HANDLER)
#define SB_SVC138_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC139_HANDLER)
#define SB_SVC139_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC140_HANDLER)
#define SB_SVC140_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC141_HANDLER)
#define SB_SVC141_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC142_HANDLER)
#define SB_SVC142_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC143_HANDLER)
#define SB_SVC143_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC144_HANDLER)
#define SB_SVC144_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC145_HANDLER)
#define SB_SVC145_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC146_HANDLER)
#define SB_SVC146_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC147_HANDLER)
#define SB_SVC147_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC148_HANDLER)
#define SB_SVC148_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC149_HANDLER)
#define SB_SVC149_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC150_HANDLER)
#define SB_SVC150_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC151_HANDLER)
#define SB_SVC151_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC152_HANDLER)
#define SB_SVC152_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC153_HANDLER)
#define SB_SVC153_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC154_HANDLER)
#define SB_SVC154_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC155_HANDLER)
#define SB_SVC155_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC156_HANDLER)
#define SB_SVC156_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC157_HANDLER)
#define SB_SVC157_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC158_HANDLER)
#define SB_SVC158_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC159_HANDLER)
#define SB_SVC159_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC160_HANDLER)
#define SB_SVC160_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC161_HANDLER)
#define SB_SVC161_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC162_HANDLER)
#define SB_SVC162_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC163_HANDLER)
#define SB_SVC163_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC164_HANDLER)
#define SB_SVC164_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC165_HANDLER)
#define SB_SVC165_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC166_HANDLER)
#define SB_SVC166_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC167_HANDLER)
#define SB_SVC167_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC168_HANDLER)
#define SB_SVC168_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC169_HANDLER)
#define SB_SVC169_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC170_HANDLER)
#define SB_SVC170_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC171_HANDLER)
#define SB_SVC171_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC172_HANDLER)
#define SB_SVC172_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC173_HANDLER)
#define SB_SVC173_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC174_HANDLER)
#define SB_SVC174_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC175_HANDLER)
#define SB_SVC175_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC176_HANDLER)
#define SB_SVC176_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC177_HANDLER)
#define SB_SVC177_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC178_HANDLER)
#define SB_SVC178_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC179_HANDLER)
#define SB_SVC179_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC180_HANDLER)
#define SB_SVC180_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC181_HANDLER)
#define SB_SVC181_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC182_HANDLER)
#define SB_SVC182_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC183_HANDLER)
#define SB_SVC183_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC184_HANDLER)
#define SB_SVC184_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC185_HANDLER)
#define SB_SVC185_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC186_HANDLER)
#define SB_SVC186_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC187_HANDLER)
#define SB_SVC187_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC188_HANDLER)
#define SB_SVC188_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC189_HANDLER)
#define SB_SVC189_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC190_HANDLER)
#define SB_SVC190_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC191_HANDLER)
#define SB_SVC191_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC192_HANDLER)
#define SB_SVC192_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC193_HANDLER)
#define SB_SVC193_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC194_HANDLER)
#define SB_SVC194_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC195_HANDLER)
#define SB_SVC195_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC196_HANDLER)
#define SB_SVC196_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC197_HANDLER)
#define SB_SVC197_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC198_HANDLER)
#define SB_SVC198_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC199_HANDLER)
#define SB_SVC199_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC200_HANDLER)
#define SB_SVC200_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC201_HANDLER)
#define SB_SVC201_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC202_HANDLER)
#define SB_SVC202_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC203_HANDLER)
#define SB_SVC203_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC204_HANDLER)
#define SB_SVC204_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC205_HANDLER)
#define SB_SVC205_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC206_HANDLER)
#define SB_SVC206_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC207_HANDLER)
#define SB_SVC207_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC208_HANDLER)
#define SB_SVC208_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC209_HANDLER)
#define SB_SVC209_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC210_HANDLER)
#define SB_SVC210_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC211_HANDLER)
#define SB_SVC211_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC212_HANDLER)
#define SB_SVC212_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC213_HANDLER)
#define SB_SVC213_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC214_HANDLER)
#define SB_SVC214_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC215_HANDLER)
#define SB_SVC215_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC216_HANDLER)
#define SB_SVC216_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC217_HANDLER)
#define SB_SVC217_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC218_HANDLER)
#define SB_SVC218_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC219_HANDLER)
#define SB_SVC219_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC220_HANDLER)
#define SB_SVC220_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC221_HANDLER)
#define SB_SVC221_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC222_HANDLER)
#define SB_SVC222_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC223_HANDLER)
#define SB_SVC223_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC224_HANDLER)
#define SB_SVC224_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC225_HANDLER)
#define SB_SVC225_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC226_HANDLER)
#define SB_SVC226_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC227_HANDLER)
#define SB_SVC227_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC228_HANDLER)
#define SB_SVC228_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC229_HANDLER)
#define SB_SVC229_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC230_HANDLER)
#define SB_SVC230_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC231_HANDLER)
#define SB_SVC231_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC232_HANDLER)
#define SB_SVC232_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC233_HANDLER)
#define SB_SVC233_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC234_HANDLER)
#define SB_SVC234_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC235_HANDLER)
#define SB_SVC235_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC236_HANDLER)
#define SB_SVC236_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC237_HANDLER)
#define SB_SVC237_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC238_HANDLER)
#define SB_SVC238_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC239_HANDLER)
#define SB_SVC239_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC240_HANDLER)
#define SB_SVC240_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC241_HANDLER)
#define SB_SVC241_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC242_HANDLER)
#define SB_SVC242_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC243_HANDLER)
#define SB_SVC243_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC244_HANDLER)
#define SB_SVC244_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC245_HANDLER)
#define SB_SVC245_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC246_HANDLER)
#define SB_SVC246_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC247_HANDLER)
#define SB_SVC247_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC248_HANDLER)
#define SB_SVC248_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC249_HANDLER)
#define SB_SVC249_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC250_HANDLER)
#define SB_SVC250_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC251_HANDLER)
#define SB_SVC251_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC252_HANDLER)
#define SB_SVC252_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC253_HANDLER)
#define SB_SVC253_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC254_HANDLER)
#define SB_SVC254_HANDLER       sb_undef_handler
#endif
#if !defined(SB_SVC255_HANDLER)
#define SB_SVC255_HANDLER       sb_undef_handler
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void port_syscall(struct port_extctx *ctxp, uint32_t n);
  void sbObjectInit(sb_class_t *sbcp);
  void sbStart(sb_class_t *sbcp, const sb_config_t *config);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SBHOST_H */

/** @} */
