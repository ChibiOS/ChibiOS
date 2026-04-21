/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    SDMMCv2/hal_sdc_lld.h
 * @brief   STM32 SDC subsystem low level driver header.
 *
 * @addtogroup SDC
 * @{
 */

#ifndef HAL_SDC_LLD_H
#define HAL_SDC_LLD_H

#if HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
#if !defined(STM32_SDC_USE_SDMMC1) || defined(__DOXYGEN__)
#define STM32_SDC_USE_SDMMC1                FALSE
#endif

#if !defined(STM32_SDC_USE_SDMMC2) || defined(__DOXYGEN__)
#define STM32_SDC_USE_SDMMC2                FALSE
#endif

#if !defined(STM32_SDC_SDMMC_UNALIGNED_SUPPORT) || defined(__DOXYGEN__)
#define STM32_SDC_SDMMC_UNALIGNED_SUPPORT   TRUE
#endif

#if !defined(STM32_SDC_SDMMC_WRITE_TIMEOUT) || defined(__DOXYGEN__)
#define STM32_SDC_SDMMC_WRITE_TIMEOUT       10000
#endif

#if !defined(STM32_SDC_SDMMC_READ_TIMEOUT) || defined(__DOXYGEN__)
#define STM32_SDC_SDMMC_READ_TIMEOUT        10000
#endif

#if !defined(STM32_SDC_SDMMC_CLOCK_DELAY) || defined(__DOXYGEN__)
#define STM32_SDC_SDMMC_CLOCK_DELAY         10
#endif

#if !defined(STM32_SDC_SDMMC_PWRSAV) || defined(__DOXYGEN__)
#define STM32_SDC_SDMMC_PWRSAV              TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (STM32_SDC_USE_SDMMC1 && !defined(STM32_SDMMC1_HANDLER)) ||             \
    (STM32_SDC_USE_SDMMC2 && !defined(STM32_SDMMC2_HANDLER))
#error "STM32_SDMMCx_HANDLER not defined in registry"
#endif

#if (STM32_SDC_USE_SDMMC1 && !defined(STM32_SDMMC1_NUMBER)) ||              \
    (STM32_SDC_USE_SDMMC2 && !defined(STM32_SDMMC2_NUMBER))
#error "STM32_SDMMCx_NUMBER not defined in registry"
#endif

#if STM32_SDC_USE_SDMMC1 && !STM32_HAS_SDMMC1
#error "SDMMC1 not present in the selected device"
#endif

#if STM32_SDC_USE_SDMMC2 && !STM32_HAS_SDMMC2
#error "SDMMC2 not present in the selected device"
#endif

#if !STM32_SDC_USE_SDMMC1 && !STM32_SDC_USE_SDMMC2
#error "SDC driver activated but no SDMMC peripheral assigned"
#endif

#if STM32_HAS_SDMMC1 && !defined(STM32_SDMMC1CLK)
#error "STM32_SDMMC1CLK not defined"
#endif

#if STM32_HAS_SDMMC2 && !defined(STM32_SDMMC2CLK)
#error "STM32_SDMMC2CLK not defined"
#endif

#if !defined(STM32_HCLK)
#error "STM32_HCLK not defined"
#endif

#if !defined(STM32_SDMMC_MAXCLK)
#error "STM32_SDMMC_MAXCLK not defined in stm32_limits.h"
#endif

#if STM32_HAS_SDMMC1 && (STM32_SDMMC1CLK > STM32_SDMMC_MAXCLK)
#error "STM32_SDMMC1CLK must not exceed STM32_SDMMC_MAXCLK"
#endif

#if STM32_HAS_SDMMC2 && (STM32_SDMMC2CLK > STM32_SDMMC_MAXCLK)
#error "STM32_SDMMC2CLK must not exceed STM32_SDMMC_MAXCLK"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

#define sdc_lld_config_fields                                               \
  /* No platform-specific configuration fields.*/

#define sdc_lld_driver_fields                                               \
  uint8_t                   *buf;                                           \
  uint32_t                  *resp;                                          \
  uint32_t                  transfer_blocks;                                \
  SDMMC_TypeDef             *sdmmc;                                         \
  uint32_t                  clkfreq

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sdc_lld_init(void);
  msg_t sdc_lld_start(hal_sdc_driver_c *sdcp);
  void sdc_lld_stop(hal_sdc_driver_c *sdcp);
  const hal_sdc_config_t *sdc_lld_setcfg(hal_sdc_driver_c *sdcp,
                                         const hal_sdc_config_t *config);
  const hal_sdc_config_t *sdc_lld_selcfg(hal_sdc_driver_c *sdcp,
                                         unsigned cfgnum);
  void sdc_lld_set_callback(hal_sdc_driver_c *sdcp, drv_cb_t cb);
  void sdc_lld_start_clk(hal_sdc_driver_c *sdcp);
  void sdc_lld_set_data_clk(hal_sdc_driver_c *sdcp, sdcbusclk_t clk);
  void sdc_lld_stop_clk(hal_sdc_driver_c *sdcp);
  void sdc_lld_set_bus_mode(hal_sdc_driver_c *sdcp, sdcbusmode_t mode);
  void sdc_lld_send_cmd_none(hal_sdc_driver_c *sdcp, uint8_t cmd,
                             uint32_t arg);
  bool sdc_lld_send_cmd_short(hal_sdc_driver_c *sdcp, uint8_t cmd,
                              uint32_t arg, uint32_t *resp);
  bool sdc_lld_send_cmd_short_crc(hal_sdc_driver_c *sdcp, uint8_t cmd,
                                  uint32_t arg, uint32_t *resp);
  bool sdc_lld_send_cmd_long_crc(hal_sdc_driver_c *sdcp, uint8_t cmd,
                                 uint32_t arg, uint32_t *resp);
  bool sdc_lld_read_special(hal_sdc_driver_c *sdcp, uint8_t *buf, size_t bytes,
                            uint8_t cmd, uint32_t arg);
  msg_t sdc_lld_start_read(hal_sdc_driver_c *sdcp, uint32_t startblk,
                           uint8_t *buf, uint32_t blocks);
  msg_t sdc_lld_start_write(hal_sdc_driver_c *sdcp, uint32_t startblk,
                            const uint8_t *buf, uint32_t blocks);
  msg_t sdc_lld_stop_transfer(hal_sdc_driver_c *sdcp);
  bool sdc_lld_sync(hal_sdc_driver_c *sdcp);
  bool sdc_lld_is_card_inserted(hal_sdc_driver_c *sdcp);
  bool sdc_lld_is_write_protected(hal_sdc_driver_c *sdcp);
  void sdc_lld_serve_interrupt(hal_sdc_driver_c *sdcp);

#if STM32_SDC_USE_SDMMC1 || defined(__DOXYGEN__)
  extern hal_sdc_driver_c SDCD1;
#endif
#if STM32_SDC_USE_SDMMC2 || defined(__DOXYGEN__)
  extern hal_sdc_driver_c SDCD2;
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SDC */

#endif /* HAL_SDC_LLD_H */

/** @} */
