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
 * @file    SDMMCv2/hal_sdc_lld.c
 * @brief   STM32 SDC subsystem low level driver source.
 *
 * @addtogroup SDC
 * @{
 */

#include "hal.h"

#if HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define SDMMC_ICR_ALL_FLAGS     0xFFFFFFFFU

#define MMCSD_BLOCK_SIZE        512U
#define MMCSD_R1_ERROR_MASK     0xFDFFE008U
#define MMCSD_STS_TRAN          4U
#define MMCSD_CMD_SEND_STATUS   13U
#define MMCSD_CMD_STOP_TRANSMISSION 12U
#define MMCSD_CMD_READ_SINGLE_BLOCK 17U
#define MMCSD_CMD_READ_MULTIPLE_BLOCK 18U
#define MMCSD_CMD_WRITE_BLOCK   24U
#define MMCSD_CMD_WRITE_MULTIPLE_BLOCK 25U

#define MMCSD_R1_ERROR(resp)    (((resp) & MMCSD_R1_ERROR_MASK) != 0U)

#define SDMMC_STA_ERROR_MASK                                                \
  (SDMMC_STA_CCRCFAIL | SDMMC_STA_DCRCFAIL |                                \
   SDMMC_STA_CTIMEOUT | SDMMC_STA_DTIMEOUT |                                \
   SDMMC_STA_TXUNDERR | SDMMC_STA_RXOVERR)

#if defined(STM32H5XX) || defined(STM32U3XX)
#define IDMABASE0 IDMABASER
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if STM32_SDC_USE_SDMMC1 || defined(__DOXYGEN__)
hal_sdc_driver_c SDCD1;
#endif

#if STM32_SDC_USE_SDMMC2 || defined(__DOXYGEN__)
hal_sdc_driver_c SDCD2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const hal_sdc_config_t sdc_default_cfg = {
  SDC_MODE_4BIT,
  0U
};

#if STM32_SDC_USE_SDMMC1 || defined(__DOXYGEN__)
static uint8_t __nocache_sd1_buf[MMCSD_BLOCK_SIZE];
static uint32_t __nocache_sd1_wbuf[1];
#endif

#if STM32_SDC_USE_SDMMC2 || defined(__DOXYGEN__)
static uint8_t __nocache_sd2_buf[MMCSD_BLOCK_SIZE];
static uint32_t __nocache_sd2_wbuf[1];
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static uint32_t sdc_lld_clkdiv(hal_sdc_driver_c *sdcp, uint32_t f) {

#if defined(STM32_SDC_MAX_CLOCK)
  if (f > STM32_SDC_MAX_CLOCK) {
    f = STM32_SDC_MAX_CLOCK;
  }
#endif

  if (f >= sdcp->clkfreq) {
    return ((const hal_sdc_config_t *)sdcp->config)->slowdown;
  }

  return ((const hal_sdc_config_t *)sdcp->config)->slowdown +
         ((sdcp->clkfreq + (f * 2U) - 1U) / (f * 2U));
}

static bool sdc_lld_wait_for_transfer_state(hal_sdc_driver_c *sdcp) {
  uint32_t resp[1];

  while (true) {
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SEND_STATUS, sdcp->rca,
                                   resp)) {
      return HAL_FAILED;
    }
    if (MMCSD_R1_ERROR(resp[0])) {
      return HAL_FAILED;
    }
    if (((resp[0] >> 9U) & 0xFU) == MMCSD_STS_TRAN) {
      return HAL_SUCCESS;
    }
#if SDC_NICE_WAITING == TRUE
    osalThreadSleepMilliseconds(1);
#endif
  }
}

__STATIC_FORCEINLINE uint32_t sdc_lld_get_timeout(hal_sdc_driver_c *sdcp,
                                                  uint32_t ms) {
  uint32_t div = (sdcp->sdmmc->CLKCR & SDMMC_CLKCR_CLKDIV_Msk) + 1U;
  return (((sdcp->clkfreq / (div * 2U)) / 1000U) * ms);
}

static void sdc_lld_collect_errors(hal_sdc_driver_c *sdcp, uint32_t sta) {
  uint32_t errors = SDC_NO_ERROR;

  if ((sta & SDMMC_STA_CCRCFAIL) != 0U) {
    errors |= SDC_CMD_CRC_ERROR;
  }
  if ((sta & SDMMC_STA_DCRCFAIL) != 0U) {
    errors |= SDC_DATA_CRC_ERROR;
  }
  if ((sta & SDMMC_STA_CTIMEOUT) != 0U) {
    errors |= SDC_COMMAND_TIMEOUT;
  }
  if ((sta & SDMMC_STA_DTIMEOUT) != 0U) {
    errors |= SDC_DATA_TIMEOUT;
  }
  if ((sta & SDMMC_STA_TXUNDERR) != 0U) {
    errors |= SDC_TX_UNDERRUN;
  }
  if ((sta & SDMMC_STA_RXOVERR) != 0U) {
    errors |= SDC_RX_OVERRUN;
  }

  sdcp->errors |= errors;
}

static void sdc_lld_end_data_path(hal_sdc_driver_c *sdcp) {
  sdcp->sdmmc->IDMACTRL = 0U;
  sdcp->sdmmc->MASK     = 0U;
  sdcp->sdmmc->DCTRL    = 0U;
}

static bool sdc_lld_prepare_read_bytes(hal_sdc_driver_c *sdcp,
                                       uint8_t *buf, uint32_t bytes) {
  osalDbgCheck(bytes < 0x1000000U);

  sdcp->sdmmc->DTIMER = sdc_lld_get_timeout(sdcp, STM32_SDC_SDMMC_READ_TIMEOUT);

  if (sdc_lld_wait_for_transfer_state(sdcp)) {
    return HAL_FAILED;
  }

  sdcp->sdmmc->IDMABASE0 = (uint32_t)buf;
  sdcp->sdmmc->IDMACTRL  = SDMMC_IDMA_IDMAEN;

  sdcp->sdmmc->ICR   = SDMMC_ICR_ALL_FLAGS;
  sdcp->sdmmc->MASK  = SDMMC_MASK_DCRCFAILIE |
                       SDMMC_MASK_DTIMEOUTIE |
                       SDMMC_MASK_RXOVERRIE |
                       SDMMC_MASK_DATAENDIE;
  sdcp->sdmmc->DLEN  = bytes;

  sdcp->sdmmc->DCTRL = SDMMC_DCTRL_DTDIR |
                       SDMMC_DCTRL_FIFORST |
                       SDMMC_DCTRL_DTMODE_0 |
                       SDMMC_DCTRL_DTEN;

  return HAL_SUCCESS;
}

static bool sdc_lld_prepare_read(hal_sdc_driver_c *sdcp, uint32_t startblk,
                                 uint32_t n, uint32_t *resp) {

  if ((sdcp->cardmode & SDC_MODE_HIGH_CAPACITY) == 0U) {
    startblk *= MMCSD_BLOCK_SIZE;
  }

  if (n > 1U) {
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_READ_MULTIPLE_BLOCK,
                                   startblk, resp) || MMCSD_R1_ERROR(resp[0])) {
      return HAL_FAILED;
    }
  }
  else {
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_READ_SINGLE_BLOCK,
                                   startblk, resp) || MMCSD_R1_ERROR(resp[0])) {
      return HAL_FAILED;
    }
  }

  return HAL_SUCCESS;
}

static bool sdc_lld_prepare_write(hal_sdc_driver_c *sdcp, uint32_t startblk,
                                  uint32_t n, uint32_t *resp) {

  if ((sdcp->cardmode & SDC_MODE_HIGH_CAPACITY) == 0U) {
    startblk *= MMCSD_BLOCK_SIZE;
  }

  if (n > 1U) {
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_WRITE_MULTIPLE_BLOCK,
                                   startblk, resp) || MMCSD_R1_ERROR(resp[0])) {
      return HAL_FAILED;
    }
  }
  else {
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_WRITE_BLOCK,
                                   startblk, resp) || MMCSD_R1_ERROR(resp[0])) {
      return HAL_FAILED;
    }
  }

  return HAL_SUCCESS;
}

static void sdc_lld_error_cleanup(hal_sdc_driver_c *sdcp,
                                  uint32_t blocks,
                                  uint32_t *resp) {
  uint32_t sta;

  sta = sdcp->sdmmc->STA;
  sdcp->sdmmc->ICR = sta;
  sdc_lld_end_data_path(sdcp);
  sdc_lld_collect_errors(sdcp, sta);

  if (blocks > 1U) {
    (void)sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_STOP_TRANSMISSION, 0U,
                                     resp);
  }
}

static bool sdc_lld_wait_transaction_end_poll(hal_sdc_driver_c *sdcp,
                                              uint32_t blocks,
                                              uint32_t *resp) {
  uint32_t sta;

  while (((sta = sdcp->sdmmc->STA) &
          (SDMMC_STA_DATAEND | SDMMC_STA_ERROR_MASK)) == 0U) {
  }

  sdc_lld_end_data_path(sdcp);

  if ((sta & SDMMC_STA_DATAEND) == 0U) {
    sdcp->sdmmc->ICR = sta;
    sdc_lld_collect_errors(sdcp, sta);
    return HAL_FAILED;
  }

  sdcp->sdmmc->ICR = SDMMC_ICR_ALL_FLAGS;

  if (blocks > 1U) {
    return sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_STOP_TRANSMISSION, 0U,
                                      resp);
  }

  return HAL_SUCCESS;
}

static msg_t sdc_lld_start_transfer(hal_sdc_driver_c *sdcp, uint32_t startblk,
                                    const uint8_t *buf, uint32_t blocks,
                                    bool readop) {
  uint32_t *resp = sdcp->resp;

  if ((((uintptr_t)buf) & 3U) != 0U) {
    return HAL_RET_CONFIG_ERROR;
  }

  osalDbgCheck(blocks < 0x1000000U / MMCSD_BLOCK_SIZE);

  sdcp->sdmmc->DTIMER = sdc_lld_get_timeout(sdcp,
                                            readop ?
                                            STM32_SDC_SDMMC_READ_TIMEOUT :
                                            STM32_SDC_SDMMC_WRITE_TIMEOUT);

  if (sdc_lld_wait_for_transfer_state(sdcp)) {
    return HAL_RET_HW_FAILURE;
  }

  sdcp->sdmmc->IDMABASE0 = (uint32_t)buf;
  sdcp->sdmmc->IDMACTRL  = SDMMC_IDMA_IDMAEN;

  sdcp->sdmmc->ICR  = SDMMC_ICR_ALL_FLAGS;
  sdcp->sdmmc->DLEN = blocks * MMCSD_BLOCK_SIZE;
  if (readop) {
    sdcp->sdmmc->MASK = SDMMC_MASK_DCRCFAILIE |
                        SDMMC_MASK_DTIMEOUTIE |
                        SDMMC_MASK_RXOVERRIE |
                        SDMMC_MASK_DATAENDIE;
    if (sdc_lld_prepare_read(sdcp, startblk, blocks, resp)) {
      sdc_lld_error_cleanup(sdcp, blocks, resp);
      return HAL_RET_HW_FAILURE;
    }
    sdcp->sdmmc->DCTRL = SDMMC_DCTRL_DTDIR |
                         SDMMC_DCTRL_FIFORST |
                         SDMMC_DCTRL_DBLOCKSIZE_3 |
                         SDMMC_DCTRL_DBLOCKSIZE_0 |
                         SDMMC_DCTRL_DTEN;
  }
  else {
    sdcp->sdmmc->MASK = SDMMC_MASK_DCRCFAILIE |
                        SDMMC_MASK_DTIMEOUTIE |
                        SDMMC_MASK_TXUNDERRIE |
                        SDMMC_MASK_DATAENDIE;
    if (sdc_lld_prepare_write(sdcp, startblk, blocks, resp)) {
      sdc_lld_error_cleanup(sdcp, blocks, resp);
      return HAL_RET_HW_FAILURE;
    }
    sdcp->sdmmc->DCTRL = SDMMC_DCTRL_FIFORST |
                         SDMMC_DCTRL_DBLOCKSIZE_3 |
                         SDMMC_DCTRL_DBLOCKSIZE_0 |
                         SDMMC_DCTRL_DTEN;
  }

  sdcp->transfer_blocks = blocks;

  return HAL_RET_SUCCESS;
}

static msg_t sdc_lld_finish_transfer(hal_sdc_driver_c *sdcp) {
  uint32_t sta;
  uint32_t blocks = sdcp->transfer_blocks;

  sdc_lld_end_data_path(sdcp);

  sta = sdcp->sdmmc->STA;
  sdcp->sdmmc->ICR = SDMMC_ICR_ALL_FLAGS;

  if ((sta & SDMMC_STA_DATAEND) == 0U) {
    sdc_lld_collect_errors(sdcp, sta);
    sdcp->transfer_blocks = 0U;
    return HAL_RET_HW_FAILURE;
  }

  if ((sta & SDMMC_STA_ERROR_MASK) != 0U) {
    sdc_lld_collect_errors(sdcp, sta);
    sdcp->transfer_blocks = 0U;
    return HAL_RET_HW_FAILURE;
  }

  if (blocks > 1U) {
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_STOP_TRANSMISSION, 0U,
                                   sdcp->resp)) {
      sdcp->transfer_blocks = 0U;
      return HAL_RET_HW_FAILURE;
    }
  }

  sdcp->transfer_blocks = 0U;
  return HAL_RET_SUCCESS;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void sdc_lld_init(void) {

#if STM32_SDC_USE_SDMMC1
  sdcObjectInit(&SDCD1);
  SDCD1.sdmmc = SDMMC1;
  SDCD1.clkfreq = STM32_SDMMC1CLK;
  SDCD1.buf = __nocache_sd1_buf;
  SDCD1.resp = __nocache_sd1_wbuf;
  SDCD1.transfer_blocks = 0U;
#endif

#if STM32_SDC_USE_SDMMC2
  sdcObjectInit(&SDCD2);
  SDCD2.sdmmc = SDMMC2;
  SDCD2.clkfreq = STM32_SDMMC2CLK;
  SDCD2.buf = __nocache_sd2_buf;
  SDCD2.resp = __nocache_sd2_wbuf;
  SDCD2.transfer_blocks = 0U;
#endif
}

msg_t sdc_lld_start(hal_sdc_driver_c *sdcp) {
  const hal_sdc_config_t *config;

  config = (const hal_sdc_config_t *)sdcp->config;
  if (config == NULL) {
    config = sdc_lld_selcfg(sdcp, 0U);
    if (config == NULL) {
      config = &sdc_default_cfg;
    }
    sdcp->config = config;
  }

  {
#if STM32_SDC_USE_SDMMC1
    if (&SDCD1 == sdcp) {
      rccEnableSDMMC1(true);
    }
#endif
#if STM32_SDC_USE_SDMMC2
    if (&SDCD2 == sdcp) {
      rccEnableSDMMC2(true);
    }
#endif
  }

  sdcp->sdmmc->IDMACTRL = 0U;
  sdcp->sdmmc->DCTRL    = 0U;
  sdcp->sdmmc->POWER    = 0U;
  sdcp->sdmmc->CLKCR    = 0U;
  sdcp->sdmmc->DTIMER   = 0U;
  sdcp->sdmmc->ICR      = SDMMC_ICR_ALL_FLAGS;
  sdcp->transfer_blocks = 0U;

  return HAL_RET_SUCCESS;
}

void sdc_lld_stop(hal_sdc_driver_c *sdcp) {

  if (sdcp->state != HAL_DRV_STATE_STOP) {
    sdc_lld_end_data_path(sdcp);
    sdcp->sdmmc->POWER  = 0U;
    sdcp->sdmmc->CLKCR  = 0U;
    sdcp->sdmmc->DTIMER = 0U;
    sdcp->sdmmc->ICR    = SDMMC_ICR_ALL_FLAGS;
    sdcp->transfer_blocks = 0U;

#if STM32_SDC_USE_SDMMC1
    if (&SDCD1 == sdcp) {
      rccResetSDMMC1();
      rccDisableSDMMC1();
    }
#endif
#if STM32_SDC_USE_SDMMC2
    if (&SDCD2 == sdcp) {
      rccResetSDMMC2();
      rccDisableSDMMC2();
    }
#endif
  }
}

const hal_sdc_config_t *sdc_lld_setcfg(hal_sdc_driver_c *sdcp,
                                       const hal_sdc_config_t *config) {
  (void)sdcp;

  if (config == NULL) {
    return sdc_lld_selcfg(sdcp, 0U);
  }

  return config;
}

const hal_sdc_config_t *sdc_lld_selcfg(hal_sdc_driver_c *sdcp,
                                       unsigned cfgnum) {
  (void)sdcp;

#if SDC_USE_CONFIGURATIONS == TRUE
  extern const sdc_configurations_t sdc_configurations;

  if (cfgnum < sdc_configurations.cfgsnum) {
    return &sdc_configurations.cfgs[cfgnum];
  }
#else
  (void)cfgnum;
#endif

  return NULL;
}

void sdc_lld_set_callback(hal_sdc_driver_c *sdcp, drv_cb_t cb) {
  (void)sdcp;
  (void)cb;
}

void sdc_lld_start_clk(hal_sdc_driver_c *sdcp) {
  sdcp->sdmmc->CLKCR  = sdc_lld_clkdiv(sdcp, 400000U);
  sdcp->sdmmc->POWER |= SDMMC_POWER_PWRCTRL_0 | SDMMC_POWER_PWRCTRL_1;
  osalThreadSleep(OSAL_MS2I(STM32_SDC_SDMMC_CLOCK_DELAY));
}

void sdc_lld_set_data_clk(hal_sdc_driver_c *sdcp, sdcbusclk_t clk) {

  if (clk == SDC_CLK_50MHz) {
    sdcp->sdmmc->CLKCR = (sdcp->sdmmc->CLKCR & ~(SDMMC_CLKCR_PWRSAV_Msk |
                                                 SDMMC_CLKCR_CLKDIV_Msk)) |
#if STM32_SDC_SDMMC_PWRSAV
                         sdc_lld_clkdiv(sdcp, 50000000U) | SDMMC_CLKCR_PWRSAV;
#else
                         sdc_lld_clkdiv(sdcp, 50000000U);
#endif
  }
  else {
    sdcp->sdmmc->CLKCR = (sdcp->sdmmc->CLKCR & ~(SDMMC_CLKCR_PWRSAV_Msk |
                                                 SDMMC_CLKCR_CLKDIV_Msk)) |
#if STM32_SDC_SDMMC_PWRSAV
                         sdc_lld_clkdiv(sdcp, 25000000U) | SDMMC_CLKCR_PWRSAV;
#else
                         sdc_lld_clkdiv(sdcp, 25000000U);
#endif
  }
}

void sdc_lld_stop_clk(hal_sdc_driver_c *sdcp) {
  sdcp->sdmmc->CLKCR = 0U;
  sdcp->sdmmc->POWER = 0U;
}

void sdc_lld_set_bus_mode(hal_sdc_driver_c *sdcp, sdcbusmode_t mode) {
  uint32_t clk = sdcp->sdmmc->CLKCR & ~SDMMC_CLKCR_WIDBUS;

  switch (mode) {
  case SDC_MODE_1BIT:
    sdcp->sdmmc->CLKCR = clk;
    break;
  case SDC_MODE_4BIT:
    sdcp->sdmmc->CLKCR = clk | SDMMC_CLKCR_WIDBUS_0;
    break;
  case SDC_MODE_8BIT:
    sdcp->sdmmc->CLKCR = clk | SDMMC_CLKCR_WIDBUS_1;
    break;
  default:
    break;
  }
}

void sdc_lld_send_cmd_none(hal_sdc_driver_c *sdcp, uint8_t cmd, uint32_t arg) {

  sdcp->sdmmc->ARG = arg;
  sdcp->sdmmc->CMD = (uint32_t)cmd | SDMMC_CMD_CPSMEN;
  while ((sdcp->sdmmc->STA & SDMMC_STA_CMDSENT) == 0U) {
  }
  sdcp->sdmmc->ICR = SDMMC_ICR_CMDSENTC;
}

bool sdc_lld_send_cmd_short(hal_sdc_driver_c *sdcp, uint8_t cmd,
                            uint32_t arg, uint32_t *resp) {
  uint32_t sta;

  sdcp->sdmmc->ARG = arg;
  sdcp->sdmmc->CMD = (uint32_t)cmd | SDMMC_CMD_WAITRESP_0 | SDMMC_CMD_CPSMEN;
  while (((sta = sdcp->sdmmc->STA) & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                                      SDMMC_STA_CCRCFAIL)) == 0U) {
  }
  sdcp->sdmmc->ICR = sta & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                            SDMMC_STA_CCRCFAIL);
  if ((sta & SDMMC_STA_CTIMEOUT) != 0U) {
    sdc_lld_collect_errors(sdcp, sta);
    return HAL_FAILED;
  }
  *resp = sdcp->sdmmc->RESP1;
  return HAL_SUCCESS;
}

bool sdc_lld_send_cmd_short_crc(hal_sdc_driver_c *sdcp, uint8_t cmd,
                                uint32_t arg, uint32_t *resp) {
  uint32_t sta;

  sdcp->sdmmc->ARG = arg;
  sdcp->sdmmc->CMD = (uint32_t)cmd | SDMMC_CMD_WAITRESP_0 | SDMMC_CMD_CPSMEN;
  while (((sta = sdcp->sdmmc->STA) & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                                      SDMMC_STA_CCRCFAIL)) == 0U) {
  }
  sdcp->sdmmc->ICR = sta & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                            SDMMC_STA_CCRCFAIL);
  if ((sta & (SDMMC_STA_CTIMEOUT | SDMMC_STA_CCRCFAIL)) != 0U) {
    sdc_lld_collect_errors(sdcp, sta);
    return HAL_FAILED;
  }
  *resp = sdcp->sdmmc->RESP1;
  return HAL_SUCCESS;
}

bool sdc_lld_send_cmd_long_crc(hal_sdc_driver_c *sdcp, uint8_t cmd,
                               uint32_t arg, uint32_t *resp) {
  uint32_t sta;

  sdcp->sdmmc->ARG = arg;
  sdcp->sdmmc->CMD = (uint32_t)cmd | SDMMC_CMD_WAITRESP_0 |
                                    SDMMC_CMD_WAITRESP_1 |
                                    SDMMC_CMD_CPSMEN;
  while (((sta = sdcp->sdmmc->STA) & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                                      SDMMC_STA_CCRCFAIL)) == 0U) {
  }
  sdcp->sdmmc->ICR = sta & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                            SDMMC_STA_CCRCFAIL);
  if ((sta & SDMMC_STA_ERROR_MASK) != 0U) {
    sdc_lld_collect_errors(sdcp, sta);
    return HAL_FAILED;
  }
  *resp++ = sdcp->sdmmc->RESP4;
  *resp++ = sdcp->sdmmc->RESP3;
  *resp++ = sdcp->sdmmc->RESP2;
  *resp   = sdcp->sdmmc->RESP1;
  return HAL_SUCCESS;
}

bool sdc_lld_read_special(hal_sdc_driver_c *sdcp, uint8_t *buf, size_t bytes,
                          uint8_t cmd, uint32_t arg) {

  if (sdc_lld_prepare_read_bytes(sdcp, buf, bytes)) {
    goto error;
  }

  if (sdc_lld_send_cmd_short_crc(sdcp, cmd, arg, sdcp->resp) ||
      MMCSD_R1_ERROR(sdcp->resp[0])) {
    goto error;
  }

  if (sdc_lld_wait_transaction_end_poll(sdcp, 1U, sdcp->resp)) {
    goto error;
  }

  return HAL_SUCCESS;

error:
  sdc_lld_error_cleanup(sdcp, 1U, sdcp->resp);
  return HAL_FAILED;
}

msg_t sdc_lld_start_read(hal_sdc_driver_c *sdcp, uint32_t startblk,
                         uint8_t *buf, uint32_t blocks) {
  return sdc_lld_start_transfer(sdcp, startblk, buf, blocks, true);
}

msg_t sdc_lld_start_write(hal_sdc_driver_c *sdcp, uint32_t startblk,
                          const uint8_t *buf, uint32_t blocks) {
  return sdc_lld_start_transfer(sdcp, startblk, buf, blocks, false);
}

msg_t sdc_lld_stop_transfer(hal_sdc_driver_c *sdcp) {
  uint32_t blocks = sdcp->transfer_blocks;

  sdc_lld_end_data_path(sdcp);
  sdcp->sdmmc->ICR = SDMMC_ICR_ALL_FLAGS;
  if (blocks > 1U) {
    (void)sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_STOP_TRANSMISSION, 0U,
                                     sdcp->resp);
  }
  sdcp->transfer_blocks = 0U;

  return HAL_RET_SUCCESS;
}

bool sdc_lld_sync(hal_sdc_driver_c *sdcp) {
  (void)sdcp;
  return HAL_SUCCESS;
}

void sdc_lld_serve_interrupt(hal_sdc_driver_c *sdcp) {
  msg_t msg;

  osalSysLockFromISR();
  msg = sdc_lld_finish_transfer(sdcp);
  if (msg == HAL_RET_SUCCESS) {
    _sdc_isr_complete_code(sdcp);
  }
  else {
    _sdc_isr_error_code(sdcp);
  }
  osalSysUnlockFromISR();
}

#endif /* HAL_USE_SDC */

/** @} */
