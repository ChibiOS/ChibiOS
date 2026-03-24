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
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const flash_descriptor_t efl_lld_descriptor = {
  .attributes       = FLASH_ATTR_ERASED_IS_ONE |
                      FLASH_ATTR_MEMORY_MAPPED |
                      FLASH_ATTR_REWRITABLE,
  .page_size        = RP_FLASH_PAGE_SIZE,
  .sectors_count    = RP_FLASH_SECTORS_COUNT,
  .sectors          = NULL,
  .sectors_size     = RP_FLASH_SECTOR_SIZE,
  .address          = (uint8_t *)RP_FLASH_BASE,
  .size             = RP_FLASH_SIZE
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

/**
 * @brief   Low level Embedded Flash driver initialization.
 *
 * @notapi
 */
void efl_lld_init(void) {

  /* Driver initialization. */
  eflObjectInit(&EFLD1);
}

/**
 * @brief   Configures and activates the Embedded Flash peripheral.
 *
 * @param[in] eflp      pointer to a @p EFlashDriver structure
 *
 * @notapi
 */
void efl_lld_start(EFlashDriver *eflp) {

  /* Copy the boot2 stage2 image (first 252 bytes of flash, excluding
   * the 4-byte CRC) into a RAM buffer while XIP is still functional.
   * After flash program/erase operations, rp_flash_enter_xip()
   * re-executes this copy to restore the SSI/XIP configuration.
   * See RP2040 Datasheet section 2.8.1.3 for boot2 requirements. */
  memcpy(eflp->boot2, (const void *)RP_FLASH_BASE, sizeof(eflp->boot2));
}

/**
 * @brief   Deactivates the Embedded Flash peripheral.
 *
 * @param[in] eflp      pointer to a @p EFlashDriver structure
 *
 * @notapi
 */
void efl_lld_stop(EFlashDriver *eflp) {

  (void)eflp;

  /* Nothing to do. */
}

/**
 * @brief   Gets the flash descriptor structure.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @return              A flash device descriptor.
 *
 * @notapi
 */
const flash_descriptor_t *efl_lld_get_descriptor(void *instance) {

  (void)instance;

  return &efl_lld_descriptor;
}

/**
 * @brief   Read operation.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @param[in] offset    offset within full flash address space
 * @param[in] n         number of bytes to be read
 * @param[out] rp       pointer to the data buffer
 * @return              An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_READ         if the read operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_read(void *instance, flash_offset_t offset,
                           size_t n, uint8_t *rp) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  flash_error_t err = FLASH_NO_ERROR;

  osalDbgCheck((instance != NULL) && (rp != NULL) && (n > 0U));
  osalDbgCheck((size_t)offset + n <= (size_t)efl_lld_descriptor.size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No reading while erasing. */
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* FLASH_READ state while the operation is performed. */
  devp->state = FLASH_READ;

  /* Read from memory-mapped XIP region. */
  memcpy((void *)rp, (const void *)(efl_lld_descriptor.address + offset), n);

  /* Ready state again. */
  devp->state = FLASH_READY;

  return err;
}

/**
 * @brief   Program operation.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @param[in] offset    offset within full flash address space
 * @param[in] n         number of bytes to be programmed
 * @param[in] pp        pointer to the data buffer
 * @return              An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_PROGRAM      if the program operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_program(void *instance, flash_offset_t offset,
                              size_t n, const uint8_t *pp) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  syssts_t sts;

  osalDbgCheck((instance != NULL) && (pp != NULL) && (n > 0U));
  osalDbgCheck((size_t)offset + n <= (size_t)efl_lld_descriptor.size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No programming while erasing. */
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* FLASH_PGM state while the operation is performed. */
  devp->state = FLASH_PGM;

  /* Program in page-sized chunks.  The source data copy is intentionally
   * done outside the system lock while XIP is still enabled; only the
   * RAM-resident page transaction itself is bracketed by syslock. */
  while (n > 0U) {
    uint8_t page_buf[RP_FLASH_PAGE_SIZE];
    size_t page_offset = offset & RP_FLASH_PAGE_MASK;
    size_t page_remaining = RP_FLASH_PAGE_SIZE - page_offset;
    size_t chunk = (n < page_remaining) ? n : page_remaining;

    /* Copy to RAM while flash is still readable. */
    memcpy(page_buf, pp, chunk);

    sts = osalSysGetStatusAndLockX();

    /* Program the page. */
    rp_flash_program_page_full(devp, offset, page_buf, chunk);

    osalSysRestoreStatusX(sts);

    offset += chunk;
    pp += chunk;
    n -= chunk;
  }

  /* Ready state again. */
  devp->state = FLASH_READY;

  return FLASH_NO_ERROR;
}

/**
 * @brief   Starts a whole-device erase operation.
 * @note    This is not implemented for safety reasons - erasing the entire
 *          flash would destroy the running firmware.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @return              An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_all(void *instance) {

  (void)instance;

  return FLASH_ERROR_UNIMPLEMENTED;
}

/**
 * @brief   Starts a sector erase operation.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @param[in] sector    sector to be erased
 * @return              An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_sector(void *instance,
                                         flash_sector_t sector) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  flash_offset_t offset;
  syssts_t sts;

  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < efl_lld_descriptor.sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No erasing while erasing. */
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* FLASH_ERASE state while the operation is performed. */
  devp->state = FLASH_ERASE;

  /* Calculate sector offset. */
  offset = sector * RP_FLASH_SECTOR_SIZE;

  /* Lock the system around the single RAM-resident erase sequence. */
  sts = osalSysGetStatusAndLockX();

  /* Perform the entire erase sequence in RAM. */
  rp_flash_erase_full(devp, FLASHCMD_SECTOR_ERASE, offset);

  /* Restore system state. */
  osalSysRestoreStatusX(sts);

  /* Back to ready state. */
  devp->state = FLASH_READY;

  return FLASH_NO_ERROR;
}

/**
 * @brief   Starts a block erase operation.
 *
 * @param[in] instance    pointer to a @p EFlashDriver instance
 * @param[in] cmd         JEDEC erase command byte
 * @param[in] erase_size  erase unit size in bytes
 * @param[in] block       block number to be erased
 * @return                An error code.
 * @retval FLASH_NO_ERROR           if the block erase completed.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_block(void *instance,
                                        uint8_t cmd,
                                        uint32_t erase_size,
                                        uint32_t block) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  flash_offset_t offset;
  syssts_t sts;

  osalDbgCheck(instance != NULL);
  osalDbgCheck(block < (RP_FLASH_SIZE / erase_size));
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  devp->state = FLASH_ERASE;

  offset = block * erase_size;

  /* UID read is one uninterrupted RAM-resident XIP-off transaction, so the
   * whole helper runs under syslock. */
  sts = osalSysGetStatusAndLockX();
  rp_flash_erase_full(devp, cmd, offset);
  osalSysRestoreStatusX(sts);

  devp->state = FLASH_READY;

  return FLASH_NO_ERROR;
}

/**
 * @brief   Queries the driver for erase operation progress.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @param[out] msec     recommended time, in milliseconds, that
 *                      should be spent before calling this
 *                      function again, can be @p NULL
 * @return              An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_ERASE        if the erase operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_query_erase(void *instance, uint32_t *msec) {
  EFlashDriver *devp = (EFlashDriver *)instance;

  /* If there is an erase in progress then the device must be checked. */
  if (devp->state == FLASH_ERASE) {
    /*
     * Note: Our implementation waits for erase completion in
     * efl_lld_start_erase_sector(), so we should never actually
     * be in FLASH_ERASE state when this is called. However, we
     * handle it properly for interface compliance.
     */
    if (msec != NULL) {
      *msec = RP_FLASH_WAIT_TIME_MS;
    }
    return FLASH_BUSY_ERASING;
  }

  return FLASH_NO_ERROR;
}

/**
 * @brief   Returns the erase state of a sector.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @param[in] sector    sector to be verified
 * @return              An error code.
 * @retval FLASH_NO_ERROR           if the sector is erased.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_VERIFY       if the verify operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_verify_erase(void *instance, flash_sector_t sector) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  const uint32_t *address;
  flash_error_t err = FLASH_NO_ERROR;
  unsigned i;

  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < efl_lld_descriptor.sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No verifying while erasing. */
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* Address of the sector in XIP space. */
  address = (const uint32_t *)(efl_lld_descriptor.address +
                               flashGetSectorOffset(getBaseFlash(devp), sector));

  /* FLASH_READ state while the operation is performed. */
  devp->state = FLASH_READ;

  /* Scanning the sector space. */
  for (i = 0U; i < RP_FLASH_SECTOR_SIZE / sizeof(uint32_t); i++) {
    if (address[i] != 0xFFFFFFFFU) {
      err = FLASH_ERROR_VERIFY;
      break;
    }
  }

  /* Ready state again. */
  devp->state = FLASH_READY;

  return err;
}

/**
 * @brief   Reads the flash chip's unique ID.
 * @note    The JEDEC 0x4B command requires 4 dummy bytes before the
 *          8-byte unique ID. The memcpy runs after XIP is restored
 *          so it is safe to call flash-resident libc.
 *
 * @param[in] eflp      pointer to a @p EFlashDriver structure
 * @param[out] uid      pointer to an 8-byte buffer for the unique ID
 *
 * @api
 */
void efl_lld_read_unique_id(EFlashDriver *eflp, uint8_t *uid) {
  uint8_t rx[4U + RP_FLASH_UNIQUE_ID_SIZE];
  syssts_t sts;

  osalDbgCheck((eflp != NULL) && (uid != NULL));

  sts = osalSysGetStatusAndLockX();
  rp_flash_read_uid_full(eflp, rx, sizeof(rx));
  osalSysRestoreStatusX(sts);

  memcpy(uid, rx + 4U, RP_FLASH_UNIQUE_ID_SIZE);
}

#endif /* HAL_USE_EFL == TRUE */

/** @} */
