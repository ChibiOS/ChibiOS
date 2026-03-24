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
 * @file    RP2040/hal_efl_lld.c
 * @brief   RP2040 Embedded Flash subsystem low level driver source.
 * @note    This is a self-contained flash driver that directly manipulates
 *          the SSI peripheral.
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
 * @name    SSI register offsets
 * @{
 */
#define SSI_CTRLR0                          0x00U
#define SSI_CTRLR1                          0x04U
#define SSI_SSIENR                          0x08U
#define SSI_SER                             0x10U
#define SSI_BAUDR                           0x14U
#define SSI_TXFLR                           0x20U
#define SSI_RXFLR                           0x24U
#define SSI_SR                              0x28U
#define SSI_ICR                             0x48U
#define SSI_DR0                             0x60U
#define SSI_SPI_CTRLR0                      0xF4U
/** @} */

/**
 * @name    SSI status register bits
 * @{
 */
#define SSI_SR_TFE                          (1U << 2)   /* TX FIFO empty */
#define SSI_SR_BUSY                         (1U << 0)   /* SSI busy */
/** @} */

/**
 * @name    SSI CTRLR0 bits
 * @{
 */
#define SSI_CTRLR0_DFS_32_POS               16U
#define SSI_CTRLR0_DFS_32_8BIT              (7U << SSI_CTRLR0_DFS_32_POS)
/** @} */

/**
 * @name    SSI configuration for direct SPI mode
 * @{
 */
#define SSI_BAUDR_DEFAULT                   6U
/** @} */

/**
 * @name    IO QSPI base and register offsets
 * @{
 */
#define RP_IOQSPI_BASE                      0x40018000U
#define IOQSPI_GPIO_QSPI_SS_CTRL            0x0CU
/** @} */

/**
 * @name    IO QSPI output override values
 * @{
 */
#define IOQSPI_OUTOVER_NORMAL               0U
#define IOQSPI_OUTOVER_LOW                  2U
#define IOQSPI_OUTOVER_HIGH                 3U
/** @} */

/**
 * @name    XIP control register offsets
 * @{
 */
#define XIP_CTRL                            0x00U
#define XIP_CTRL_EN                         1U
#define XIP_FLUSH                           0x04U
#define XIP_STAT                            0x08U
/** @} */

/**
 * @name    PADS QSPI base and register offsets
 * @{
 */
#define RP_PADS_QSPI_BASE                   0x40020000U
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
  .ssi = (volatile uint32_t *)RP_SSI_BASE
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
  (void)eflp;

  volatile uint32_t *ioqspi_ss_ctrl =
      (volatile uint32_t *)(RP_IOQSPI_BASE + IOQSPI_GPIO_QSPI_SS_CTRL);
  uint32_t val = high ? IOQSPI_OUTOVER_HIGH : IOQSPI_OUTOVER_LOW;

  *ioqspi_ss_ctrl = (*ioqspi_ss_ctrl & ~(3U << 8)) | (val << 8);

  /* Read back to ensure write is flushed */
  (void)*ioqspi_ss_ctrl;
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
  volatile uint32_t *ssi = eflp->ssi;
  size_t tx_remaining = count;
  size_t rx_remaining = count;
  const size_t max_in_flight = 14U; /* FIFO is 16 deep so we leave a margin */

  while ((tx_remaining > 0U) || (rx_remaining > 0U)) {
    size_t in_flight = (count - tx_remaining) - (count - rx_remaining);

    while ((tx_remaining > 0U) && (in_flight < max_in_flight)) {
      uint8_t data = (tx != NULL) ? *tx++ : 0U;
      ssi[SSI_DR0 / 4U] = data;
      tx_remaining--;
      in_flight++;
    }

    while ((rx_remaining > 0U) && (ssi[SSI_RXFLR / 4U] > 0U)) {
      uint8_t data = (uint8_t)ssi[SSI_DR0 / 4U];
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
  /* Assert CS. */
  rp_flash_cs_force(eflp, false);

  /* Send command byte. */
  eflp->ssi[SSI_DR0 / 4U] = cmd;
  while (eflp->ssi[SSI_RXFLR / 4U] == 0U) {
  }
  (void)eflp->ssi[SSI_DR0 / 4U];

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
 * @brief   Flush XIP cache
 * @note    This function MUST be in RAM.
 */
RAMFUNC static void rp_flash_flush_cache(void) {
  volatile uint32_t *xip = (volatile uint32_t *)RP_XIP_CTRL_BASE;
  uint32_t ctrl;

  /* Write to flush register to trigger cache flush. */
  xip[XIP_FLUSH / 4U] = 1U;

  /* Read back blocks until flush is complete. */
  (void)xip[XIP_FLUSH / 4U];

  /* Re-enable the cache while preserving the remaining policy bits. */
  ctrl = xip[XIP_CTRL / 4U];
  ctrl |= XIP_CTRL_EN;
  xip[XIP_CTRL / 4U] = ctrl;
}

/**
 * @brief   Exit XIP mode and configure for direct access.
 * @note    This function MUST be in RAM.
 * @note    This follows a similar pattern to the ROM's flash_exit_xip()
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 */
RAMFUNC static void rp_flash_exit_xip(EFlashDriver *eflp) {
  volatile uint32_t *ssi = eflp->ssi;
  volatile uint32_t *pads_qspi = (volatile uint32_t *)RP_PADS_QSPI_BASE;
  uint32_t padctrl_save;
  uint32_t padctrl_tmp;
  unsigned i;
  volatile unsigned delay;

  /* Wait for any pending work.*/
  while ((ssi[SSI_SR / 4U] & SSI_SR_BUSY) != 0U) {
  }

  /* Default non XIP SPI configuration */
  ssi[SSI_SSIENR / 4U] = 0U;
  ssi[SSI_BAUDR / 4U] = SSI_BAUDR_DEFAULT;
  ssi[SSI_CTRLR0 / 4U] = SSI_CTRLR0_DFS_32_8BIT;
  ssi[SSI_SER / 4U] = 1U;
  ssi[SSI_SSIENR / 4U] = 1U;

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
    ssi[SSI_DR0 / 4U] = 0U;
    while (ssi[SSI_RXFLR / 4U] == 0U) {
    }
    (void)ssi[SSI_DR0 / 4U];
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
    ssi[SSI_DR0 / 4U] = 0U;
    while (ssi[SSI_RXFLR / 4U] == 0U) {
    }
    (void)ssi[SSI_DR0 / 4U];
  }

  /* Restore pad controls. */
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD0 / 4U] = padctrl_save;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD1 / 4U] = padctrl_save;
  padctrl_save = (padctrl_save & ~PADS_QSPI_PDE_BITS) | PADS_QSPI_PUE_BITS;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD2 / 4U] = padctrl_save;
  pads_qspi[PADS_QSPI_GPIO_QSPI_SD3 / 4U] = padctrl_save;

  /* 3. Send 0xFF, 0xFF */
  rp_flash_cs_force(eflp, false);
  ssi[SSI_DR0 / 4U] = 0xFFU;
  ssi[SSI_DR0 / 4U] = 0xFFU;
  while (ssi[SSI_RXFLR / 4U] < 2U) {
  }
  (void)ssi[SSI_DR0 / 4U];
  (void)ssi[SSI_DR0 / 4U];
  rp_flash_cs_force(eflp, true);
}

/**
 * @brief   Enter XIP mode by re-executing the boot2 stage2 code.
 * @note    This function MUST be in RAM.
 * @details The SSI SPI_CTRLR0 register XIP_CMD field (bits 31:24) does
 *          not read back on RP2040 — it always returns zero regardless
 *          of the value written.  Because the boot2 stores the QSPI
 *          continuous-read mode byte (typically 0xA0) in this field,
 *          a register save/restore approach cannot recover the original
 *          XIP configuration.
 *
 *          Instead, the boot2 stage2 code is copied to RAM at driver
 *          start and re-executed here.  This is the same technique used
 *          by the Pico SDK (flash_enable_xip_via_boot2 in
 *          hardware_flash/flash.c).
 *
 *          Per RP2040 Datasheet section 2.8.1.3, the 256-byte boot2
 *          image is position-independent Thumb code — the bootrom
 *          copies it to SRAM and calls it during the boot sequence.
 *          All standard boot2 variants (w25q080, generic_03h,
 *          at25sf128a, is25lp080) follow the Pico SDK convention of
 *          checking the saved LR on exit: if non-zero (called from
 *          user code) the boot2 returns to the caller; if zero (called
 *          from the bootrom) it jumps to the application entry point.
 *          Custom boot2 implementations that do not follow this
 *          convention will not work with this driver.
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 */
RAMFUNC static void rp_flash_enter_xip(EFlashDriver *eflp) {
  volatile uint32_t *ioqspi_ss_ctrl =
      (volatile uint32_t *)(RP_IOQSPI_BASE + IOQSPI_GPIO_QSPI_SS_CTRL);

  /* Reset CS control to normal. */
  *ioqspi_ss_ctrl = 0U;

  /* Re-execute the boot2 to fully restore the SSI configuration
   * including QSPI mode, continuous read, and baud rate.  The OR
   * with 1 sets the Thumb bit required by BX on ARMv6-M. */
  ((void (*)(void))((uintptr_t)eflp->boot2 | 1U))();

  rp_flash_flush_cache();
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
  volatile uint32_t *ssi = eflp->ssi;
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
  ssi[SSI_DR0 / 4U] = FLASHCMD_PAGE_PROGRAM;
  while (ssi[SSI_RXFLR / 4U] == 0U) {
  }
  (void)ssi[SSI_DR0 / 4U];

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

void rp_efl_lld_start(EFlashDriver *eflp) {

  /* Copy the boot2 stage2 image (first 252 bytes of flash, excluding
   * the 4-byte CRC) into a RAM buffer while XIP is still functional.
   * After flash program/erase operations, rp_flash_enter_xip()
   * re-executes this copy to restore the SSI/XIP configuration.
   * See RP2040 Datasheet section 2.8.1.3 for boot2 requirements. */
  memcpy(eflp->boot2, (const void *)RP_FLASH_BASE, sizeof(eflp->boot2));
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
