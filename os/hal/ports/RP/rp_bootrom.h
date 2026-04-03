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
#define RP_ROM_FUNC_WAIT_FOR_VECTOR RP_ROM_TABLE_CODE('W', 'V')
/** @} */

/**
 * @name    RP2040-only boot ROM data table codes
 * @{
 */
#define RP_ROM_DATA_COPYRIGHT_STRING  RP_ROM_TABLE_CODE('C', 'R')
#define RP_ROM_DATA_GIT_REVISION      RP_ROM_TABLE_CODE('G', 'R')
#define RP_ROM_DATA_SOFT_FLOAT_TABLE  RP_ROM_TABLE_CODE('S', 'F')
#define RP_ROM_DATA_SOFT_DOUBLE_TABLE RP_ROM_TABLE_CODE('S', 'D')
#define RP_ROM_DATA_FPLIB_START       RP_ROM_TABLE_CODE('F', 'S')
#define RP_ROM_DATA_FPLIB_END         RP_ROM_TABLE_CODE('F', 'E')
#define RP_ROM_DATA_FLOAT_TABLE_SIZE  RP_ROM_TABLE_CODE('F', 'Z')
/** @} */
#endif

#if defined(RP2350) || defined(__DOXYGEN__)
/**
 * @name    RP2350-only boot ROM function codes
 * @{
 */
#define RP_ROM_FUNC_REBOOT                      \
  RP_ROM_TABLE_CODE('R', 'B')
#define RP_ROM_FUNC_FLASH_OP                    \
  RP_ROM_TABLE_CODE('F', 'O')
#define RP_ROM_FUNC_FLASH_RUNTIME_TO_STORAGE_ADDR \
  RP_ROM_TABLE_CODE('F', 'A')
#define RP_ROM_FUNC_FLASH_RESET_ADDRESS_TRANS   \
  RP_ROM_TABLE_CODE('R', 'A')
#define RP_ROM_FUNC_FLASH_SELECT_XIP_READ_MODE  \
  RP_ROM_TABLE_CODE('X', 'M')
#define RP_ROM_FUNC_GET_SYS_INFO                \
  RP_ROM_TABLE_CODE('G', 'S')
#define RP_ROM_FUNC_GET_PARTITION_TABLE_INFO    \
  RP_ROM_TABLE_CODE('G', 'P')
#define RP_ROM_FUNC_PICK_AB_PARTITION           \
  RP_ROM_TABLE_CODE('A', 'B')
#define RP_ROM_FUNC_GET_B_PARTITION             \
  RP_ROM_TABLE_CODE('G', 'B')
#define RP_ROM_FUNC_GET_UF2_TARGET_PARTITION    \
  RP_ROM_TABLE_CODE('G', 'U')
#define RP_ROM_FUNC_LOAD_PARTITION_TABLE        \
  RP_ROM_TABLE_CODE('L', 'P')
#define RP_ROM_FUNC_OTP_ACCESS                  \
  RP_ROM_TABLE_CODE('O', 'A')
#define RP_ROM_FUNC_CHAIN_IMAGE                 \
  RP_ROM_TABLE_CODE('C', 'I')
#define RP_ROM_FUNC_EXPLICIT_BUY                \
  RP_ROM_TABLE_CODE('E', 'B')
#define RP_ROM_FUNC_BOOTROM_STATE_RESET         \
  RP_ROM_TABLE_CODE('S', 'R')
#define RP_ROM_FUNC_SET_BOOTROM_STACK           \
  RP_ROM_TABLE_CODE('S', 'S')
#define RP_ROM_FUNC_SECURE_CALL                 \
  RP_ROM_TABLE_CODE('S', 'C')
#define RP_ROM_FUNC_SET_NS_API_PERMISSION       \
  RP_ROM_TABLE_CODE('S', 'P')
#define RP_ROM_FUNC_SET_ROM_CALLBACK            \
  RP_ROM_TABLE_CODE('R', 'C')
#define RP_ROM_FUNC_VALIDATE_NS_BUFFER          \
  RP_ROM_TABLE_CODE('V', 'B')
/** @} */

/**
 * @name    RP2350-only boot ROM data table codes
 * @{
 */
#define RP_ROM_DATA_SOFTWARE_GIT_REVISION       \
  RP_ROM_TABLE_CODE('G', 'R')
#define RP_ROM_DATA_FLASH_DEVINFO16_PTR         \
  RP_ROM_TABLE_CODE('F', 'D')
#define RP_ROM_DATA_PARTITION_TABLE_PTR         \
  RP_ROM_TABLE_CODE('P', 'T')
#define RP_ROM_DATA_SAVED_XIP_SETUP_FUNC_PTR    \
  RP_ROM_TABLE_CODE('X', 'F')
/** @} */

/**
 * @name    RP2350 ROM table lookup flags
 * @{
 */
#define RP_ROM_RT_FLAG_FUNC_RISCV               0x0001U
#define RP_ROM_RT_FLAG_FUNC_RISCV_FAR           0x0003U
#define RP_ROM_RT_FLAG_FUNC_ARM_SEC             0x0004U
#define RP_ROM_RT_FLAG_FUNC_ARM_NONSEC          0x0010U
#define RP_ROM_RT_FLAG_DATA                     0x0040U
/** @} */

/**
 * @name    RP2350 reboot flags
 * @{
 */
#define RP_ROM_REBOOT2_FLAG_REBOOT_TYPE_NORMAL       0x00U
#define RP_ROM_REBOOT2_FLAG_REBOOT_TYPE_BOOTSEL      0x02U
#define RP_ROM_REBOOT2_FLAG_REBOOT_TYPE_RAM_IMAGE    0x03U
#define RP_ROM_REBOOT2_FLAG_REBOOT_TYPE_FLASH_UPDATE 0x04U
#define RP_ROM_REBOOT2_FLAG_REBOOT_TYPE_PC_SP        0x0DU
#define RP_ROM_REBOOT2_FLAG_REBOOT_TO_ARM            0x10U
#define RP_ROM_REBOOT2_FLAG_REBOOT_TO_RISCV          0x20U
#define RP_ROM_REBOOT2_FLAG_NO_RETURN_ON_SUCCESS     0x100U
/** @} */

/**
 * @name    RP2350 BOOTSEL flags
 * @{
 */
#define RP_ROM_BOOTSEL_GPIO_PIN_ACTIVE_LOW      0x10U
#define RP_ROM_BOOTSEL_GPIO_PIN_SPECIFIED       0x20U
/** @} */

/**
 * @name    RP2350 boot ROM error codes
 * @{
 */
#define RP_ROM_OK                               0
#define RP_ROM_ERROR_NOT_PERMITTED              (-4)
#define RP_ROM_ERROR_INVALID_ARG                (-5)
#define RP_ROM_ERROR_INVALID_ADDRESS            (-10)
#define RP_ROM_ERROR_BAD_ALIGNMENT              (-11)
#define RP_ROM_ERROR_INVALID_STATE              (-12)
#define RP_ROM_ERROR_BUFFER_TOO_SMALL           (-13)
#define RP_ROM_ERROR_PRECONDITION_NOT_MET       (-14)
#define RP_ROM_ERROR_MODIFIED_DATA              (-15)
#define RP_ROM_ERROR_INVALID_DATA               (-16)
#define RP_ROM_ERROR_NOT_FOUND                  (-17)
#define RP_ROM_ERROR_UNSUPPORTED_MODIFICATION   (-18)
#define RP_ROM_ERROR_LOCK_REQUIRED              (-19)
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
  void __attribute__((noreturn)) rpRomResetUsbBoot(
    uint32_t gpio_mask, uint32_t disable_interface_mask);
#if defined(RP2350) || defined(__DOXYGEN__)
  int rpRomReboot(uint32_t flags, uint32_t delay_ms,
                  uint32_t p0, uint32_t p1);
#endif
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
