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
 * @file    RP2350/hal_efl_lld.c
 * @brief   RP2350 Embedded Flash subsystem low level driver source.
 * @note    This is a self-contained flash driver that directly manipulates
 *          the QMI peripheral.
 *
 * @addtogroup HAL_EFL
 * @{
 */

#include <string.h>

#include "hal.h"
#include "rp_efl_lld.h"

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Macro to place functions in RAM.
 */
#define RAMFUNC __attribute__((noinline, section(".ramtext")))

/**
 * @name    QMI register offsets
 * @{
 */
#define QMI_DIRECT_CSR                      0x00U
#define QMI_DIRECT_TX                       0x04U
#define QMI_DIRECT_RX                       0x08U
#define QMI_M0_TIMING                       0x0CU
#define QMI_M0_RFMT                         0x10U
#define QMI_M0_RCMD                         0x14U
#define QMI_M0_WFMT                         0x18U
#define QMI_M0_WCMD                         0x1CU
/** @} */

/**
 * @name    QMI_DIRECT_CSR bits
 * @{
 */
#define QMI_DIRECT_CSR_EN                   (1U << 0)
#define QMI_DIRECT_CSR_BUSY                 (1U << 1)
#define QMI_DIRECT_CSR_ASSERT_CS0N          (1U << 2)
#define QMI_DIRECT_CSR_TXFULL               (1U << 10)
#define QMI_DIRECT_CSR_TXEMPTY              (1U << 11)
#define QMI_DIRECT_CSR_RXEMPTY              (1U << 16)
#define QMI_DIRECT_CSR_CLKDIV_POS           22U
#define QMI_DIRECT_CSR_CLKDIV_MASK          (0xFFU << QMI_DIRECT_CSR_CLKDIV_POS)
/** @} */

/**
 * @name    XIP cache constants
 * @{
 */
#define RP_XIP_MAINTENANCE_BASE             0x18000000U
#define RP_XIP_CACHE_LINE_SIZE              8U
#define RP_XIP_CACHE_SIZE                   (16U * 1024U)
#define RP_XIP_ADDRESS_SPACE_SIZE           0x04000000U
#define RP_XIP_SET_WAY_BASE                 (RP_XIP_ADDRESS_SPACE_SIZE - RP_XIP_CACHE_SIZE)
/** @} */

/**
 * @name    XIP cache maintenance operations
 * @{
 */
#define RP_XIP_CACHE_INVALIDATE_BY_SET_WAY  0U
#define RP_XIP_CACHE_CLEAN_BY_SET_WAY       1U
/** @} */

/**
 * @name    XIP control register offsets
 * @{
 */
#define XIP_CTRL                            0x00U
#define XIP_FLUSH                           0x04U
#define XIP_STAT                            0x08U
/** @} */

/**
 * @name    PADS QSPI base and register offsets
 * @{
 */
#define RP_PADS_QSPI_BASE                   0x40040000U
#define PADS_QSPI_GPIO_QSPI_SD0             0x08U
#define PADS_QSPI_GPIO_QSPI_SD1             0x0CU
#define PADS_QSPI_GPIO_QSPI_SD2             0x10U
#define PADS_QSPI_GPIO_QSPI_SD3             0x14U
/** @} */

/**
 * @name    PADS QSPI control bits
 * @{
 */
#define PADS_QSPI_OD_BITS                   (1U << 7)   /* Output disable */
#define PADS_QSPI_IE_BITS                   (1U << 6)   /* Input enable */
#define PADS_QSPI_PUE_BITS                  (1U << 3)   /* Pull up enable */
#define PADS_QSPI_PDE_BITS                  (1U << 2)   /* Pull down enable */
/** @} */

/**
 * @name    Standard JEDEC Flash commands
 * @{
 */
#define FLASHCMD_WRITE_ENABLE               0x06U
#define FLASHCMD_READ_STATUS                0x05U
#define FLASHCMD_PAGE_PROGRAM               0x02U
#define FLASHCMD_SECTOR_ERASE               0x20U
#define FLASHCMD_BLOCK_ERASE_32K            0x52U
#define FLASHCMD_BLOCK_ERASE_64K            0xD8U
#define FLASHCMD_READ_UNIQUE_ID             0x4BU
/** @} */

/**
 * @name    Flash status register bits
 * @{
 */
#define FLASH_STATUS_BUSY                   (1U << 0)
#define FLASH_STATUS_WEL                    (1U << 1)
/** @} */

/**
 * @name    Page alignment
 * @{
 */
#define RP_FLASH_PAGE_MASK                  (RP_FLASH_PAGE_SIZE - 1U)
/** @} */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EFL1 driver identifier.
 * @note    EFLD1.ssi is statically initialized to allow use before hal init
 */
EFlashDriver EFLD1 = {
  .qmi = (volatile uint32_t *)RP_QMI_BASE
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Force chip select high or low
 * @note    This function MUST be in RAM.
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 * @param[in] high      true for CS high (deassert), false for CS low (assert)
 */
RAMFUNC static void rp_flash_cs_force(EFlashDriver *eflp, bool high) {
  volatile uint32_t *qmi = eflp->qmi;

  if (high) {
    qmi[QMI_DIRECT_CSR / 4U] &= ~QMI_DIRECT_CSR_ASSERT_CS0N;
  } else {
    qmi[QMI_DIRECT_CSR / 4U] |= QMI_DIRECT_CSR_ASSERT_CS0N;
  }

  /* Read back to ensure write is flushed */
  (void)qmi[QMI_DIRECT_CSR / 4U];
}

/**
 * @brief   Transfer data to/from flash
 * @note    This function MUST be in RAM.
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 * @param[in] tx        transmit buffer (NULL to send zeros)
 * @param[out] rx       receive buffer (NULL to discard)
 * @param[in] count     number of bytes to transfer
 */
RAMFUNC static void rp_flash_put_get(EFlashDriver *eflp, const uint8_t *tx,
                                     uint8_t *rx, size_t count) {
  volatile uint32_t *qmi = eflp->qmi;
  size_t tx_remaining = count;
  size_t rx_remaining = count;

  while ((tx_remaining > 0U) || (rx_remaining > 0U)) {
    uint32_t csr = qmi[QMI_DIRECT_CSR / 4U];

    /* Send if TX FIFO not full and data remaining. */
    if ((tx_remaining > 0U) && ((csr & QMI_DIRECT_CSR_TXFULL) == 0U)) {
      uint8_t data = (tx != NULL) ? *tx++ : 0U;
      qmi[QMI_DIRECT_TX / 4U] = data;
      tx_remaining--;
    }

    if ((rx_remaining > 0U) && ((csr & QMI_DIRECT_CSR_RXEMPTY) == 0U)) {
      uint8_t data = (uint8_t)qmi[QMI_DIRECT_RX / 4U];
      if (rx != NULL) {
        *rx++ = data;
      }
      rx_remaining--;
    }
  }
}

/**
 * @brief   Execute a flash command.
 * @note    This function MUST be in RAM.
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 * @param[in] cmd       command byte
 * @param[in] tx        transmit data after command (NULL if none)
 * @param[out] rx       receive buffer (NULL to discard)
 * @param[in] count     number of bytes to transfer after command
 */
RAMFUNC static void rp_flash_do_cmd(EFlashDriver *eflp, uint8_t cmd,
                                    const uint8_t *tx, uint8_t *rx,
                                    size_t count) {
  volatile uint32_t *qmi = eflp->qmi;

  /* Assert CS. */
  rp_flash_cs_force(eflp, false);

  /* Send command byte. */
  qmi[QMI_DIRECT_TX / 4U] = cmd;
  while ((qmi[QMI_DIRECT_CSR / 4U] & QMI_DIRECT_CSR_RXEMPTY) != 0U) {
  }
  (void)qmi[QMI_DIRECT_RX / 4U];

  /* Transfer remaining data. */
  if (count > 0U) {
    rp_flash_put_get(eflp, tx, rx, count);
  }

  /* Deassert CS. */
  rp_flash_cs_force(eflp, true);
}

/**
 * @brief   Wait for flash to become ready.
 * @note    This function MUST be in RAM.
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 */
RAMFUNC static void rp_flash_wait_ready(EFlashDriver *eflp) {
  uint8_t status;

  do {
    rp_flash_do_cmd(eflp, FLASHCMD_READ_STATUS, NULL, &status, 1U);
  } while ((status & FLASH_STATUS_BUSY) != 0U);
}

/**
 * @brief   Send write enable command.
 * @note    This function MUST be in RAM.
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 */
RAMFUNC static void rp_flash_write_enable(EFlashDriver *eflp) {

  rp_flash_do_cmd(eflp, FLASHCMD_WRITE_ENABLE, NULL, NULL, 0U);
}

/**
 * @brief   Flush XIP cache and restore cache policy.
 * @note    This function MUST be in RAM.
 */
RAMFUNC static void rp_flash_flush_cache(EFlashDriver *eflp) {
  volatile uint8_t *maint = (volatile uint8_t *)RP_XIP_MAINTENANCE_BASE;
  volatile uint32_t *xip = (volatile uint32_t *)RP_XIP_CTRL_BASE;
  uint32_t offset;

  /*
   * Clean before invalidate: the XIP cache is shared with PSRAM (CS1),
   * which may have dirty write-back lines. The last 16KB of the XIP
   * address space covers all cache set/way combinations.
   */
  for (offset = RP_XIP_SET_WAY_BASE;
       offset < RP_XIP_ADDRESS_SPACE_SIZE;
       offset += RP_XIP_CACHE_LINE_SIZE) {
    maint[offset + RP_XIP_CACHE_CLEAN_BY_SET_WAY] = 0U;
  }

  __DSB();
  __ISB();

  for (offset = RP_XIP_SET_WAY_BASE;
       offset < RP_XIP_ADDRESS_SPACE_SIZE;
       offset += RP_XIP_CACHE_LINE_SIZE) {
    maint[offset + RP_XIP_CACHE_INVALIDATE_BY_SET_WAY] = 0U;
  }

  __DSB();
  __ISB();

  /* Restore the saved cache policy after maintenance. */
  xip[XIP_CTRL / 4U] = eflp->xip_ctrl;
}

/**
 * @brief   Exit XIP mode and configure for direct access.
 * @note    This function MUST be in RAM.
 * @note    This follows a similar pattern to the ROM's flash_exit_xip()
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 */
RAMFUNC static void rp_flash_exit_xip(EFlashDriver *eflp) {
  volatile uint32_t *qmi = eflp->qmi;
  volatile uint32_t *xip = (volatile uint32_t *)RP_XIP_CTRL_BASE;
  volatile uint32_t *pads_qspi = (volatile uint32_t *)RP_PADS_QSPI_BASE;
  uint32_t padctrl_save;
  uint32_t padctrl_tmp;
  unsigned i;
  volatile unsigned delay;

  /* Save current XIP configuration before switching to direct mode. */
  eflp->xip_ctrl = xip[XIP_CTRL / 4U];
  eflp->xip_timing = qmi[QMI_M0_TIMING / 4U];
  eflp->xip_rfmt = qmi[QMI_M0_RFMT / 4U];
  eflp->xip_rcmd = qmi[QMI_M0_RCMD / 4U];

  /* Wait for any pending work.*/
  while ((qmi[QMI_DIRECT_CSR / 4U] & QMI_DIRECT_CSR_BUSY) != 0U) {
  }

  /* Default non XIP SPI configuration */
  qmi[QMI_DIRECT_CSR / 4U] = QMI_DIRECT_CSR_EN |
                             (6U << QMI_DIRECT_CSR_CLKDIV_POS);

  /*
   * Exit continuous read mode sequence:
   * 1. CS high 32 clocks with IO pulled down
   * 2. CS low 32 clocks with IO pulled up
   * 3. Send 0xFF, 0xFF
   */

  /* Save pad control and configure with output disabled.*/
  padctrl_save = pads_qspi[PADS_QSPI_GPIO_QSPI_SD0 / 4U];
  padctrl_tmp = (padctrl_save & ~(PADS_QSPI_OD_BITS | PADS_QSPI_PUE_BITS |
                                  PADS_QSPI_PDE_BITS))
                | PADS_QSPI_OD_BITS | PADS_QSPI_PDE_BITS;

  /* 1. CS high */
  rp_flash_cs_force(eflp, true);

  pads_qspi[PADS_QSPI_GPIO_QSPI_SD0 / 4U] = padctrl_tmp;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD1 / 4U] = padctrl_tmp;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD2 / 4U] = padctrl_tmp;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD3 / 4U] = padctrl_tmp;

  /* Delay of ~6000 cycles */
  for (delay = 0U; delay < 2048U; delay++) {
  }

  /* Send 4 bytes / 32 clocks */
  for (i = 0U; i < 4U; i++) {
    qmi[QMI_DIRECT_TX / 4U] = 0U;
    while ((qmi[QMI_DIRECT_CSR / 4U] & QMI_DIRECT_CSR_RXEMPTY) != 0U) {
    }
    (void)qmi[QMI_DIRECT_RX / 4U];
  }

  padctrl_tmp = (padctrl_tmp & ~PADS_QSPI_PDE_BITS) | PADS_QSPI_PUE_BITS;

  /* 2. CS low */
  rp_flash_cs_force(eflp, false);

  pads_qspi[PADS_QSPI_GPIO_QSPI_SD0 / 4U] = padctrl_tmp;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD1 / 4U] = padctrl_tmp;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD2 / 4U] = padctrl_tmp;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD3 / 4U] = padctrl_tmp;

  /* Delay of ~6000 cycles */
  for (delay = 0U; delay < 2048U; delay++) {
  }

  /* Send 4 bytes / 32 clocks */
  for (i = 0U; i < 4U; i++) {
    qmi[QMI_DIRECT_TX / 4U] = 0U;
    while ((qmi[QMI_DIRECT_CSR / 4U] & QMI_DIRECT_CSR_RXEMPTY) != 0U) {
    }
    (void)qmi[QMI_DIRECT_RX / 4U];
  }

  /* Restore pad controls. */
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD0 / 4U] = padctrl_save;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD1 / 4U] = padctrl_save;
  padctrl_save = (padctrl_save & ~PADS_QSPI_PDE_BITS) | PADS_QSPI_PUE_BITS;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD2 / 4U] = padctrl_save;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD3 / 4U] = padctrl_save;

  /* 3. Send 0xFF, 0xFF */
  rp_flash_cs_force(eflp, false);
  qmi[QMI_DIRECT_TX / 4U] = 0xFFU;
  qmi[QMI_DIRECT_TX / 4U] = 0xFFU;
  while ((qmi[QMI_DIRECT_CSR / 4U] & QMI_DIRECT_CSR_RXEMPTY) != 0U) {
  }
  (void)qmi[QMI_DIRECT_RX / 4U];
  while ((qmi[QMI_DIRECT_CSR / 4U] & QMI_DIRECT_CSR_RXEMPTY) != 0U) {
  }
  (void)qmi[QMI_DIRECT_RX / 4U];
  rp_flash_cs_force(eflp, true);
}

/**
 * @brief   Enter XIP mode
 * @note    This function MUST be in RAM.
 * @note    Restores the XIP configuration that was saved when exit_xip
 *          was called, preserving whatever mode the bootrom configured
 *          (e.g. QSPI fast read).
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 */
RAMFUNC static void rp_flash_enter_xip(EFlashDriver *eflp) {
  volatile uint32_t *qmi = eflp->qmi;

  /* Wait for transfers to complete */
  while ((qmi[QMI_DIRECT_CSR / 4U] & QMI_DIRECT_CSR_BUSY) != 0U) {
  }

  /* Disable direct mode and restore saved XIP configuration. */
  qmi[QMI_DIRECT_CSR / 4U] = 0U;
  qmi[QMI_M0_TIMING / 4U] = eflp->xip_timing;
  qmi[QMI_M0_RFMT / 4U] = eflp->xip_rfmt;
  qmi[QMI_M0_RCMD / 4U] = eflp->xip_rcmd;

  rp_flash_flush_cache(eflp);
}

/**
 * @brief   Program a page of flash.
 * @note    This function MUST be in RAM.
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 * @param[in] offset    flash offset (must be page-aligned or within page)
 * @param[in] data      data to program
 * @param[in] len       number of bytes (must not cross page boundary)
 */
RAMFUNC static void rp_flash_program_page(EFlashDriver *eflp, uint32_t offset,
                                          const uint8_t *data, size_t len) {
  volatile uint32_t *qmi = eflp->qmi;
  uint8_t addr[3];

  /* Send write enable. */
  rp_flash_write_enable(eflp);

  /* Prepare 24-bit address (big-endian). */
  addr[0] = (uint8_t)(offset >> 16);
  addr[1] = (uint8_t)(offset >> 8);
  addr[2] = (uint8_t)offset;

  /* Assert CS. */
  rp_flash_cs_force(eflp, false);

  /* Send page program command. */
  qmi[QMI_DIRECT_TX / 4U] = FLASHCMD_PAGE_PROGRAM;
  while ((qmi[QMI_DIRECT_CSR / 4U] & QMI_DIRECT_CSR_RXEMPTY) != 0U) {
  }
  (void)qmi[QMI_DIRECT_RX / 4U];

  /* Send address. */
  rp_flash_put_get(eflp, addr, NULL, 3U);

  /* Send data. */
  rp_flash_put_get(eflp, data, NULL, len);

  /* Deassert CS. */
  rp_flash_cs_force(eflp, true);

  /* Wait for program to complete. */
  rp_flash_wait_ready(eflp);
}

/**
 * @brief   Send an erase command to flash.
 * @note    This function MUST be in RAM.
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 * @param[in] cmd       JEDEC erase command byte
 * @param[in] offset    flash offset (must be aligned to erase unit)
 */
RAMFUNC static void rp_flash_erase_cmd(EFlashDriver *eflp, uint8_t cmd,
                                        uint32_t offset) {
  uint8_t addr[3];

  /* Send write enable. */
  rp_flash_write_enable(eflp);

  /* Prepare 24-bit address (big-endian). */
  addr[0] = (uint8_t)(offset >> 16);
  addr[1] = (uint8_t)(offset >> 8);
  addr[2] = (uint8_t)offset;

  /* Send erase command with address. */
  rp_flash_do_cmd(eflp, cmd, addr, NULL, 3U);
}

/**
 * @brief   Complete erase operation (runs entirely in RAM).
 * @note    This function MUST be in RAM. It handles the entire sequence
 *          from exit XIP to enter XIP so no flash code executes while
 *          XIP is disabled.
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 * @param[in] cmd       JEDEC erase command byte
 * @param[in] offset    flash offset (must be aligned to erase unit)
 */
RAMFUNC static void rp_flash_erase_full(EFlashDriver *eflp, uint8_t cmd,
                                         uint32_t offset) {

  /* Exit XIP mode. */
  rp_flash_exit_xip(eflp);

  /* Send erase command. */
  rp_flash_erase_cmd(eflp, cmd, offset);

  /* Wait for erase to complete. */
  rp_flash_wait_ready(eflp);

  /* Re-enter XIP mode. */
  rp_flash_enter_xip(eflp);
}

/**
 * @brief   Single-page program operation that runs entirely in RAM.
 * @note    This function MUST be in RAM. It handles the entire sequence
 *          from exit XIP to enter XIP so no flash code executes while
 *          XIP is disabled.
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 * @param[in] offset    flash offset (must not cross page boundary)
 * @param[in] data      pointer to data in RAM
 * @param[in] len       number of bytes to program
 */
RAMFUNC static void rp_flash_program_page_full(EFlashDriver *eflp,
                                               uint32_t offset,
                                               const uint8_t *data,
                                               size_t len) {

  /* Exit XIP mode. */
  rp_flash_exit_xip(eflp);

  /* Program the page. */
  rp_flash_program_page(eflp, offset, data, len);

  /* Re-enter XIP mode. */
  rp_flash_enter_xip(eflp);
}

/**
 * @brief   Read flash unique ID (runs entirely in RAM).
 * @note    This function MUST be in RAM. It handles the entire sequence
 *          from exit XIP to enter XIP so no flash code executes while
 *          XIP is disabled.
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 * @param[out] rx       receive buffer
 * @param[in] count     number of bytes to transfer after command
 */
RAMFUNC static void rp_flash_read_uid_full(EFlashDriver *eflp,
                                            uint8_t *rx, size_t count) {

  /* Exit XIP mode. */
  rp_flash_exit_xip(eflp);

  /* Send read unique ID command. */
  rp_flash_do_cmd(eflp, FLASHCMD_READ_UNIQUE_ID, NULL, rx, count);

  /* Re-enter XIP mode. */
  rp_flash_enter_xip(eflp);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void rp_efl_lld_init(void) {

  /* Nothing to do - RP2350 uses QMI register save/restore. */
}

void rp_efl_lld_start(EFlashDriver *eflp) {

  (void)eflp;

  /* Nothing to do - flash is always accessible via XIP. */
}
void rp_efl_lld_program_page_full(EFlashDriver *eflp,
                                  uint32_t offset,
                                  const uint8_t *data,
                                  size_t len) {
  rp_flash_program_page_full(eflp, offset, data, len);
}

void rp_efl_lld_erase_full(EFlashDriver *eflp, uint8_t cmd, uint32_t offset) {
  rp_flash_erase_full(eflp, cmd, offset);
}

void rp_efl_lld_read_uid_full(EFlashDriver *eflp,
                              uint8_t *rx,
                              size_t count) {
  rp_flash_read_uid_full(eflp, rx, count);
}

#endif /* HAL_USE_EFL == TRUE */

/** @} */
