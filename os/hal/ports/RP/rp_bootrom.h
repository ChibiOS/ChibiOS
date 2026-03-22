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
 * @file    rp_bootrom.h
 * @brief   RP boot ROM access API header.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef RP_BOOTROM_H
#define RP_BOOTROM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Builds a boot ROM lookup code from two ASCII characters.
 */
#define RP_ROM_TABLE_CODE(c1, c2) \
  ((uint32_t)(c1) | ((uint32_t)(c2) << 8))

/**
 * @name    Boot ROM function codes available on RP2040 and RP2350
 * @{
 */
#define RP_ROM_FUNC_FLASH_ENTER_CMD_XIP \
  RP_ROM_TABLE_CODE('C', 'X')
#define RP_ROM_FUNC_FLASH_EXIT_XIP \
  RP_ROM_TABLE_CODE('E', 'X')
#define RP_ROM_FUNC_FLASH_FLUSH_CACHE \
  RP_ROM_TABLE_CODE('F', 'C')
#define RP_ROM_FUNC_CONNECT_INTERNAL_FLASH \
  RP_ROM_TABLE_CODE('I', 'F')
#define RP_ROM_FUNC_FLASH_RANGE_ERASE \
  RP_ROM_TABLE_CODE('R', 'E')
#define RP_ROM_FUNC_FLASH_RANGE_PROGRAM \
  RP_ROM_TABLE_CODE('R', 'P')
/** @} */

/**
 * @name    USB BOOTSEL interface flags
 * @{
 */
#define RP_ROM_BOOTSEL_DISABLE_MSD_INTERFACE      0x01U
#define RP_ROM_BOOTSEL_DISABLE_PICOBOOT_INTERFACE 0x02U
/** @} */

#if defined(RP2040) || defined(__DOXYGEN__)
/**
 * @name    RP2040-only boot ROM function codes
 * @{
 */
#define RP_ROM_FUNC_MEMCPY44      RP_ROM_TABLE_CODE('C', '4')
#define RP_ROM_FUNC_CLZ32         RP_ROM_TABLE_CODE('L', '3')
#define RP_ROM_FUNC_MEMCPY        RP_ROM_TABLE_CODE('M', 'C')
#define RP_ROM_FUNC_MEMSET        RP_ROM_TABLE_CODE('M', 'S')
#define RP_ROM_FUNC_POPCOUNT32    RP_ROM_TABLE_CODE('P', '3')
#define RP_ROM_FUNC_REVERSE32     RP_ROM_TABLE_CODE('R', '3')
#define RP_ROM_FUNC_MEMSET4       RP_ROM_TABLE_CODE('S', '4')
#define RP_ROM_FUNC_CTZ32         RP_ROM_TABLE_CODE('T', '3')
#define RP_ROM_FUNC_RESET_USB_BOOT RP_ROM_TABLE_CODE('U', 'B')
/** @} */
#endif

#if defined(RP2350) || defined(__DOXYGEN__)
/**
 * @name    RP2350-only boot ROM function codes
 * @{
 */
#define RP_ROM_FUNC_REBOOT        RP_ROM_TABLE_CODE('R', 'B')
/** @} */
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

#if !defined(__ASSEMBLER__)

typedef void (*rp_rom_connect_internal_flash_fn_t)(void);
typedef void (*rp_rom_flash_exit_xip_fn_t)(void);
typedef void (*rp_rom_flash_range_erase_fn_t)(uint32_t offset,
                                              size_t count,
                                              uint32_t block_size,
                                              uint8_t block_cmd);
typedef void (*rp_rom_flash_range_program_fn_t)(uint32_t offset,
                                                const uint8_t *data,
                                                size_t count);
typedef void (*rp_rom_flash_flush_cache_fn_t)(void);
typedef void (*rp_rom_flash_enter_cmd_xip_fn_t)(void);
typedef void __attribute__((noreturn)) (*rp_rom_reset_usb_boot_fn_t)(
  uint32_t gpio_mask, uint32_t disable_interface_mask);

/**
 * @brief   Shared RP flash helper functions exported by the boot ROM.
 */
typedef struct {
  rp_rom_connect_internal_flash_fn_t  connect_internal_flash;
  rp_rom_flash_exit_xip_fn_t          flash_exit_xip;
  rp_rom_flash_range_erase_fn_t       flash_range_erase;
  rp_rom_flash_range_program_fn_t     flash_range_program;
  rp_rom_flash_flush_cache_fn_t       flash_flush_cache;
  rp_rom_flash_enter_cmd_xip_fn_t     flash_enter_cmd_xip;
} rp_rom_flash_api_t;

#if defined(RP2040) || defined(__DOXYGEN__)
typedef uint32_t (*rp_rom_clz32_fn_t)(uint32_t value);
typedef uint32_t (*rp_rom_ctz32_fn_t)(uint32_t value);
typedef uint32_t (*rp_rom_popcount32_fn_t)(uint32_t value);
typedef uint32_t (*rp_rom_reverse32_fn_t)(uint32_t value);
typedef void *(*rp_rom_memcpy_fn_t)(void *dest, const void *src, size_t n);
typedef uint32_t *(*rp_rom_memcpy44_fn_t)(uint32_t *dest,
                                          const uint32_t *src,
                                          uint32_t n);
typedef void *(*rp_rom_memset_fn_t)(void *dest, int c, size_t n);
typedef uint32_t *(*rp_rom_memset4_fn_t)(uint32_t *dest,
                                         uint8_t value,
                                         uint32_t n);

/**
 * @brief   RP2040 memory helper functions exported by the boot ROM.
 */
typedef struct {
  rp_rom_memcpy_fn_t    memcpy;
  rp_rom_memcpy44_fn_t  memcpy44;
  rp_rom_memset_fn_t    memset;
  rp_rom_memset4_fn_t   memset4;
} rp_rom_mem_api_t;

/**
 * @brief   RP2040 bit helper functions exported by the boot ROM.
 */
typedef struct {
  rp_rom_clz32_fn_t       clz32;
  rp_rom_ctz32_fn_t       ctz32;
  rp_rom_popcount32_fn_t  popcount32;
  rp_rom_reverse32_fn_t   reverse32;
} rp_rom_bit_api_t;
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void *rpRomFuncLookupX(uint32_t code);
  void *rpRomDataLookupX(uint32_t code);
  bool rpRomFuncsLookupX(uintptr_t *table, unsigned count);
  bool rpRomGetFlashApiX(rp_rom_flash_api_t *apip);
  bool rpRomSupportsUsbBootX(void);
  void __attribute__((noreturn)) rpRomResetUsbBoot(
    uint32_t gpio_mask, uint32_t disable_interface_mask);
#if defined(RP2040) || defined(__DOXYGEN__)
  bool rpRomGetMemApiX(rp_rom_mem_api_t *apip);
  bool rpRomGetBitApiX(rp_rom_bit_api_t *apip);
#endif
#ifdef __cplusplus
}
#endif

#endif /* !__ASSEMBLER__ */

#endif /* RP_BOOTROM_H */

/** @} */
