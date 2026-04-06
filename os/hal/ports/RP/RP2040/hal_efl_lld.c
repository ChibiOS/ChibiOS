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
 * @name    SSI configuration for direct SPI mode
 * @{
 */
#define SSI_BAUDR_DEFAULT                   6U
/** @} */

/**
 * @name    IO QSPI register indexes
 * @{
 */
#define RP2040_IO_QSPI_SS_INDEX             1U
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
  .ssi = XIP_SSI
};

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Copy of boot2 stage2 image for XIP restoration after flash ops.
 * @details 252 bytes (full 256-byte image minus 4-byte CRC).
 *          See RP2040 Datasheet 2.8.1.3 and rp_flash_enter_xip().
 */
CC_ALIGN_DATA(4) static uint8_t rp_boot2[252];

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
  uint32_t val = high ? IOQSPI_CTRL_OUTOVER_HIGH : IOQSPI_CTRL_OUTOVER_LOW;

  IO_QSPI->GPIO[RP2040_IO_QSPI_SS_INDEX].CTRL =
      (IO_QSPI->GPIO[RP2040_IO_QSPI_SS_INDEX].CTRL &
       ~IOQSPI_CTRL_OUTOVER_Msk) |
      IOQSPI_CTRL_OUTOVER(val);

  /* Read back to ensure write is flushed */
  (void)IO_QSPI->GPIO[RP2040_IO_QSPI_SS_INDEX].CTRL;
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
  SSI_TypeDef *ssi = eflp->ssi;
  size_t tx_remaining = count;
  size_t rx_remaining = count;
  const size_t max_in_flight = 14U; /* FIFO is 16 deep so we leave a margin */

  while ((tx_remaining > 0U) || (rx_remaining > 0U)) {
    size_t in_flight = (count - tx_remaining) - (count - rx_remaining);

    while ((tx_remaining > 0U) && (in_flight < max_in_flight)) {
      uint8_t data = (tx != NULL) ? *tx++ : 0U;
      ssi->DR[0] = data;
      tx_remaining--;
      in_flight++;
    }

    while ((rx_remaining > 0U) && (ssi->RXFLR > 0U)) {
      uint8_t data = (uint8_t)ssi->DR[0];
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
  eflp->ssi->DR[0] = cmd;
  while (eflp->ssi->RXFLR == 0U) {
  }
  (void)eflp->ssi->DR[0];

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
  uint32_t ctrl;

  /* Write to flush register to trigger cache flush. */
  XIP_CTRL->FLUSH = 1U;

  /* Read back blocks until flush is complete. */
  (void)XIP_CTRL->FLUSH;

  /* Re-enable the cache while preserving the remaining policy bits. */
  ctrl = XIP_CTRL->CTRL;
  ctrl |= XIP_CTRL_CTRL_EN;
  XIP_CTRL->CTRL = ctrl;
}

/**
 * @brief   Reset QSPI pads and mux to connect SSI to internal flash.
 * @note    This function MUST be in RAM.
 */
RAMFUNC static void rp_flash_connect_internal(void) {
  uint32_t bits = RESETS_ALLREG_IO_QSPI | RESETS_ALLREG_PADS_QSPI;
  unsigned i;

  /* Hard-reset IO_QSPI and PADS_QSPI. */
  RESETS->SET.RESET = bits;
  RESETS->CLR.RESET = bits;
  while ((RESETS->RESET_DONE & bits) != bits) {
  }

  /* Mux all QSPI GPIOs to function 0 (XIP). */
  for (i = 0U; i < 6U; i++) {
    IO_QSPI->GPIO[i].CTRL = 0U;
  }
}

/**
 * @brief   Exit XIP mode and configure for direct access.
 * @note    This function MUST be in RAM.
 * @note    This follows a similar pattern to the ROM's flash_exit_xip()
 *
 * @param[in] eflp      pointer to the EFlashDriver object
 */
RAMFUNC static void rp_flash_exit_xip(EFlashDriver *eflp) {
  SSI_TypeDef *ssi = eflp->ssi;
  PADS_QSPI_TypeDef *pads_qspi = PADS_QSPI;
  uint32_t padctrl_save;
  uint32_t padctrl_tmp;
  unsigned i;
  volatile unsigned delay;

  /* Wait for any pending work.*/
  while ((ssi->SR & SSI_SR_BUSY) != 0U) {
  }

  /* Default non XIP SPI configuration */
  ssi->SSIENR = 0U;
  (void)ssi->SR;                    /* Clear sticky errors (clear-on-read). */
  (void)ssi->ICR;
  ssi->BAUDR = SSI_BAUDR_DEFAULT;
  ssi->CTRLR0 = SSI_CTRLR0_SPI_FRF_STD |
                 SSI_CTRLR0_TMOD_TX_AND_RX |
                 SSI_CTRLR0_DFS_32(7U);
  ssi->SER = 1U;
  ssi->SSIENR = 1U;

  /*
   * Exit continuous read mode sequence:
   * 1. CS high 32 clocks with IO pulled down
   * 2. CS low 32 clocks with IO pulled up
   * 3. Send 0xFF, 0xFF
  */

  /* Save pad control and configure with output disabled.*/
  padctrl_save = pads_qspi->GPIO_QSPI_SD0;
  padctrl_tmp = (padctrl_save & ~(PADS_QSPI_OD | PADS_QSPI_PUE |
                                  PADS_QSPI_PDE))
                | PADS_QSPI_OD | PADS_QSPI_PDE;

  /* 1. CS high */
  rp_flash_cs_force(eflp, true);

  pads_qspi->GPIO_QSPI_SD0 = padctrl_tmp;
  pads_qspi->GPIO_QSPI_SD1 = padctrl_tmp;
  pads_qspi->GPIO_QSPI_SD2 = padctrl_tmp;
  pads_qspi->GPIO_QSPI_SD3 = padctrl_tmp;

  /* Delay of ~6000 cycles */
  for (delay = 0U; delay < 2048U; delay++) {
  }

  /* Send 4 bytes / 32 clocks */
  for (i = 0U; i < 4U; i++) {
    ssi->DR[0] = 0U;
    while (ssi->RXFLR == 0U) {
    }
    (void)ssi->DR[0];
  }

  padctrl_tmp = (padctrl_tmp & ~PADS_QSPI_PDE) | PADS_QSPI_PUE;

  /* 2. CS low */
  rp_flash_cs_force(eflp, false);

  pads_qspi->GPIO_QSPI_SD0 = padctrl_tmp;
  pads_qspi->GPIO_QSPI_SD1 = padctrl_tmp;
  pads_qspi->GPIO_QSPI_SD2 = padctrl_tmp;
  pads_qspi->GPIO_QSPI_SD3 = padctrl_tmp;

  /* Delay of ~6000 cycles */
  for (delay = 0U; delay < 2048U; delay++) {
  }

  /* Send 4 bytes / 32 clocks */
  for (i = 0U; i < 4U; i++) {
    ssi->DR[0] = 0U;
    while (ssi->RXFLR == 0U) {
    }
    (void)ssi->DR[0];
  }

  /* Restore pad controls. */
  pads_qspi->GPIO_QSPI_SD0 = padctrl_save;
  pads_qspi->GPIO_QSPI_SD1 = padctrl_save;
  padctrl_save = (padctrl_save & ~PADS_QSPI_PDE) | PADS_QSPI_PUE;
  pads_qspi->GPIO_QSPI_SD2 = padctrl_save;
  pads_qspi->GPIO_QSPI_SD3 = padctrl_save;

  /* 3. Send 0xFF, 0xFF */
  rp_flash_cs_force(eflp, false);
  ssi->DR[0] = 0xFFU;
  ssi->DR[0] = 0xFFU;
  while (ssi->RXFLR < 2U) {
  }
  (void)ssi->DR[0];
  (void)ssi->DR[0];
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
 *          Instead, the boot2 stage2 code is copied to RAM during
 *          driver init and re-executed here.  This is the same technique used
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
  (void)eflp;

  /* Reset CS control to normal. */
  IO_QSPI->GPIO[RP2040_IO_QSPI_SS_INDEX].CTRL =
      (IO_QSPI->GPIO[RP2040_IO_QSPI_SS_INDEX].CTRL &
       ~IOQSPI_CTRL_OUTOVER_Msk) |
      IOQSPI_CTRL_OUTOVER(IOQSPI_CTRL_OUTOVER_NORMAL);

  /* Re-execute the boot2 to fully restore the SSI configuration
   * including QSPI mode, continuous read, and baud rate.  The OR
   * with 1 sets the Thumb bit required by BX on ARMv6-M. */
  ((void (*)(void))((uintptr_t)rp_boot2 | 1U))();

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
  SSI_TypeDef *ssi = eflp->ssi;
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
  ssi->DR[0] = FLASHCMD_PAGE_PROGRAM;
  while (ssi->RXFLR == 0U) {
  }
  (void)ssi->DR[0];

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

  /* Connect SSI to flash and exit XIP mode. */
  rp_flash_connect_internal();
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

  /* Connect SSI to flash and exit XIP mode. */
  rp_flash_connect_internal();
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

  /* Connect SSI to flash and exit XIP mode. */
  rp_flash_connect_internal();
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

  /* Copy the boot2 stage2 image (first 252 bytes of flash, excluding
   * the 4-byte CRC) into a RAM buffer while XIP is still functional.
   * After flash program/erase operations, rp_flash_enter_xip()
   * re-executes this copy to restore the SSI/XIP configuration.
   * See RP2040 Datasheet section 2.8.1.3 for boot2 requirements. */
  memcpy(rp_boot2, (const void *)RP_FLASH_BASE, sizeof(rp_boot2));
}

void rp_efl_lld_start(EFlashDriver *eflp) {

  (void)eflp;

  /* Nothing to do - boot2 is copied during init. */
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
