/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file    STM32/sdc_lld.h
 * @brief   STM32 SDC subsystem low level driver header.
 *
 * @addtogroup SDC
 * @{
 */

#ifndef _SDC_LLD_H_
#define _SDC_LLD_H_

#if HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief Value to clear all interrupts flag at once.
 */
#define STM32_SDIO_ICR_ALL_FLAGS (SDIO_ICR_CCRCFAILC | SDIO_ICR_DCRCFAILC |   \
                                  SDIO_ICR_CTIMEOUTC | SDIO_ICR_DTIMEOUTC |   \
                                  SDIO_ICR_TXUNDERRC | SDIO_ICR_RXOVERRC |    \
                                  SDIO_ICR_CMDRENDC  | SDIO_ICR_CMDSENTC |    \
                                  SDIO_ICR_DATAENDC  | SDIO_ICR_STBITERRC |   \
                                  SDIO_ICR_DBCKENDC  | SDIO_ICR_SDIOITC |     \
                                  SDIO_ICR_CEATAENDC)

/**
 * @brief Mask of error flags in STA register.
 */
#define STM32_SDIO_STA_ERROR_MASK (SDIO_STA_CCRCFAIL | SDIO_STA_DCRCFAIL |    \
                                   SDIO_STA_CTIMEOUT | SDIO_STA_DTIMEOUT |    \
                                   SDIO_STA_TXUNDERR | SDIO_STA_RXOVERR)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SDIO DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_SDC_SDIO_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SDC_SDIO_DMA_PRIORITY         3
#endif

/**
 * @brief   SDIO interrupt priority level setting.
 */
#if !defined(STM32_SDC_SDIO_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SDC_SDIO_IRQ_PRIORITY         9
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !STM32_HAS_SDIO
#error "SDIO not present in the selected device"
#endif

#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif

/*
 * SDIO clock divider.
 */
#if (defined(STM32F4XX) || defined(STM32F2XX))
  #define STM32_SDIO_DIV_HS                   0
  #define STM32_SDIO_DIV_LS                   120
#elif STM32_HCLK > 48000000
  #define STM32_SDIO_DIV_HS                   1
  #define STM32_SDIO_DIV_LS                   178
#else
  #define STM32_SDIO_DIV_HS                   0
  #define STM32_SDIO_DIV_LS                   118
#endif

/**
 * @brief   SDIO data timeouts in SDIO clock cycles.
 */
#if (defined(STM32F4XX) || defined(STM32F2XX))
  #define STM32_SDC_WRITE_TIMEOUT \
    (((48000000 / (STM32_SDIO_DIV_HS + 2)) / 1000) * SDC_WRITE_TIMEOUT_MS)
  #define STM32_SDC_READ_TIMEOUT \
    (((48000000 / (STM32_SDIO_DIV_HS + 2)) / 1000) * SDC_READ_TIMEOUT_MS)
#else
  #define STM32_SDC_WRITE_TIMEOUT  \
    (((STM32_HCLK /((STM32_SDIO_DIV_HS + 2)) / 1000) * SDC_WRITE_TIMEOUT_MS)
  #define STM32_SDC_READ_TIMEOUT  \
    (((STM32_HCLK /((STM32_SDIO_DIV_HS + 2)) / 1000) * SDC_READ_TIMEOUT_MS)
#endif



/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of SDIO bus mode.
 */
typedef enum {
  SDC_MODE_1BIT = 0,
  SDC_MODE_4BIT,
  SDC_MODE_8BIT
} sdcbusmode_t;

/**
 * @brief   Type of card flags.
 */
typedef uint32_t sdcmode_t;

/**
 * @brief   Type of a structure representing an SDC driver.
 */
typedef struct SDCDriver SDCDriver;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  uint32_t dummy;
} SDCConfig;

/**
 * @brief   Structure representing an SDC driver.
 */
struct SDCDriver {
  /**
   * @brief Driver state.
   */
  sdcstate_t                state;
  /**
   * @brief Current configuration data.
   */
  const SDCConfig           *config;
  /**
   * @brief Various flags regarding the mounted card.
   */
  sdcmode_t                 cardmode;
  /**
   * @brief Errors flags.
   */
  uint32_t                  errors;
  /**
   * @brief Card CID.
   */
  uint32_t                  cid[4];
  /**
   * @brief Card CSD.
   */
  uint32_t                  csd[4];
  /**
   * @brief Card RCA.
   */
  uint32_t                  rca;
  /**
   * @brief Total number of blocks in card.
   */
  uint32_t                  capacity;
  /* End of the mandatory fields.*/
  /**
   * @brief Thread waiting for I/O completion IRQ.
   */
  Thread                    *thread;
  /**
   * @brief     DMA mode bit mask.
   */
  uint32_t                  dmamode;
  /**
   * @brief     Transmit DMA channel.
   */
  const stm32_dma_stream_t  *dma;
  /**
   * @brief     Pointer to the SDIO registers block.
   * @note      Used only for dubugging purpose.
   */
#if CH_DBG_ENABLE_ASSERTS
  SDIO_TypeDef              *sdio;
#endif
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern SDCDriver SDCD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sdc_lld_init(void);
  void sdc_lld_start(SDCDriver *sdcp);
  void sdc_lld_stop(SDCDriver *sdcp);
  void sdc_lld_start_clk(SDCDriver *sdcp);
  void sdc_lld_set_data_clk(SDCDriver *sdcp);
  void sdc_lld_stop_clk(SDCDriver *sdcp);
  void sdc_lld_set_bus_mode(SDCDriver *sdcp, sdcbusmode_t mode);
  void sdc_lld_send_cmd_none(SDCDriver *sdcp, uint8_t cmd, uint32_t arg);
  bool_t sdc_lld_send_cmd_short(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                                uint32_t *resp);
  bool_t sdc_lld_send_cmd_short_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                                    uint32_t *resp);
  bool_t sdc_lld_send_cmd_long_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                                   uint32_t *resp);
  bool_t sdc_lld_read(SDCDriver *sdcp, uint32_t startblk,
                      uint8_t *buf, uint32_t n);
  bool_t sdc_lld_write(SDCDriver *sdcp, uint32_t startblk,
                       const uint8_t *buf, uint32_t n);
  bool_t sdc_lld_is_card_inserted(SDCDriver *sdcp);
  bool_t sdc_lld_is_write_protected(SDCDriver *sdcp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SDC */

#endif /* _SDC_LLD_H_ */

/** @} */
