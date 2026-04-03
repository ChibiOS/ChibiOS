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
 * @file    rp_bootrom.c
 * @brief   RP boot ROM access API implementation.
 *
 * @addtogroup HAL
 * @{
 */

#include "hal.h"
#include "rp_bootrom.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define RP_ROM_BOOTROM_FUNC_TABLE_OFFSET     0x14U

#if defined(RP2040)
#define RP_ROM_BOOTROM_TABLE_LOOKUP_OFFSET   0x18U
#define RP_ROM_BOOTROM_DATA_TABLE_OFFSET     0x16U
#else
#define RP_ROM_BOOTROM_TABLE_LOOKUP_OFFSET   0x16U
#endif

static inline void *rp_rom_hword_ptr(uint32_t addr) {

  return (void *)(uintptr_t)(*(uint16_t *)(uintptr_t)(addr));
}

typedef void *(*rp_rom_table_lookup_2040_fn_t)(uint16_t *table, uint32_t code);

#if defined(RP2350)
typedef void *(*rp_rom_table_lookup_2350_fn_t)(uint32_t code, uint32_t mask);
typedef int (*rp_rom_reboot_fn_t)(uint32_t flags, uint32_t delay_ms,
                                  uint32_t p0, uint32_t p1);
#endif

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

#if defined(RP2350)
static uint32_t rp_rom_func_mask_x(void) {

#if defined(__riscv)
  return RP_ROM_RT_FLAG_FUNC_RISCV;
#elif defined(PORT_KERNEL_MODE) && (PORT_KERNEL_MODE == PORT_KERNEL_MODE_GUEST)
  return RP_ROM_RT_FLAG_FUNC_ARM_NONSEC;
#else
  return RP_ROM_RT_FLAG_FUNC_ARM_SEC;
#endif
}
#endif

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void *rpRomFuncLookupX(uint32_t code) {

#if defined(RP2040)
  rp_rom_table_lookup_2040_fn_t rom_table_lookup;
  uint16_t *func_table;

  rom_table_lookup =
      (rp_rom_table_lookup_2040_fn_t)rp_rom_hword_ptr(
          RP_ROM_BOOTROM_TABLE_LOOKUP_OFFSET);
  func_table = (uint16_t *)rp_rom_hword_ptr(RP_ROM_BOOTROM_FUNC_TABLE_OFFSET);

  return rom_table_lookup(func_table, code);
#else
  rp_rom_table_lookup_2350_fn_t rom_table_lookup;

  rom_table_lookup =
      (rp_rom_table_lookup_2350_fn_t)rp_rom_hword_ptr(
          RP_ROM_BOOTROM_TABLE_LOOKUP_OFFSET);

  return rom_table_lookup(code, rp_rom_func_mask_x());
#endif
}

void *rpRomDataLookupX(uint32_t code) {

#if defined(RP2040)
  rp_rom_table_lookup_2040_fn_t rom_table_lookup;
  uint16_t *data_table;

  rom_table_lookup =
      (rp_rom_table_lookup_2040_fn_t)rp_rom_hword_ptr(
          RP_ROM_BOOTROM_TABLE_LOOKUP_OFFSET);
  data_table = (uint16_t *)rp_rom_hword_ptr(RP_ROM_BOOTROM_DATA_TABLE_OFFSET);

  return rom_table_lookup(data_table, code);
#else
  rp_rom_table_lookup_2350_fn_t rom_table_lookup;

  rom_table_lookup =
      (rp_rom_table_lookup_2350_fn_t)rp_rom_hword_ptr(
          RP_ROM_BOOTROM_TABLE_LOOKUP_OFFSET);

  return rom_table_lookup(code, RP_ROM_RT_FLAG_DATA);
#endif
}

/**
 * @brief   Batch-resolves ROM function codes into function pointers.
 * @note    The @p table array is modified in place: each entry must be
 *          initialized with a ROM table code (via @p RP_ROM_TABLE_CODE)
 *          stored in a @p uintptr_t slot. On return every entry is
 *          overwritten with the resolved function pointer (or zero on
 *          lookup failure).
 *
 * @param[in,out] table   array of ROM codes, replaced with pointers
 * @param[in]     count   number of entries in @p table
 * @return              @p true if every code resolved successfully
 */
bool rpRomFuncsLookupX(uintptr_t *table, unsigned count) {
  bool ok = true;
  unsigned i;

  osalDbgCheck(table != NULL);

  for (i = 0U; i < count; i++) {
    table[i] = (uintptr_t)rpRomFuncLookupX((uint32_t)table[i]);
    if (table[i] == 0U) {
      ok = false;
    }
  }

  return ok;
}

bool rpRomGetFlashApiX(rp_rom_flash_api_t *apip) {
  uintptr_t table[] = {
    RP_ROM_FUNC_CONNECT_INTERNAL_FLASH,
    RP_ROM_FUNC_FLASH_EXIT_XIP,
    RP_ROM_FUNC_FLASH_RANGE_ERASE,
    RP_ROM_FUNC_FLASH_RANGE_PROGRAM,
    RP_ROM_FUNC_FLASH_FLUSH_CACHE,
    RP_ROM_FUNC_FLASH_ENTER_CMD_XIP
  };
  bool ok;

  osalDbgCheck(apip != NULL);

  *apip = (rp_rom_flash_api_t){0};
  ok = rpRomFuncsLookupX(table, 6U);

  apip->connect_internal_flash =
      (rp_rom_connect_internal_flash_fn_t)table[0];
  apip->flash_exit_xip = (rp_rom_flash_exit_xip_fn_t)table[1];
  apip->flash_range_erase = (rp_rom_flash_range_erase_fn_t)table[2];
  apip->flash_range_program = (rp_rom_flash_range_program_fn_t)table[3];
  apip->flash_flush_cache = (rp_rom_flash_flush_cache_fn_t)table[4];
  apip->flash_enter_cmd_xip = (rp_rom_flash_enter_cmd_xip_fn_t)table[5];

  return ok;
}

#if defined(RP2350)
int rpRomReboot(uint32_t flags, uint32_t delay_ms,
                uint32_t p0, uint32_t p1) {
  rp_rom_reboot_fn_t func;

  func = (rp_rom_reboot_fn_t)rpRomFuncLookupX(RP_ROM_FUNC_REBOOT);
  osalDbgAssert(func != NULL, "reboot unavailable");

  return func(flags, delay_ms, p0, p1);
}
#endif

void __attribute__((noreturn)) rpRomResetUsbBoot(
  uint32_t gpio_mask, uint32_t disable_interface_mask) {

  osalDbgAssert((gpio_mask == 0U) ||
                ((gpio_mask & (gpio_mask - 1U)) == 0U),
                "single GPIO bit required");

#if defined(RP2040)
  rp_rom_reset_usb_boot_fn_t func;

  func = (rp_rom_reset_usb_boot_fn_t)rpRomFuncLookupX(
      RP_ROM_FUNC_RESET_USB_BOOT);
  osalDbgAssert(func != NULL, "USB boot reset unavailable");

  func(gpio_mask, disable_interface_mask);
#else
  uint32_t flags = disable_interface_mask;
  uint32_t gpio = 0U;

  if (gpio_mask != 0U) {
    flags |= RP_ROM_BOOTSEL_GPIO_PIN_SPECIFIED;
    gpio = (uint32_t)__builtin_ctz(gpio_mask);
  }

  (void)rpRomReboot(RP_ROM_REBOOT2_FLAG_REBOOT_TYPE_BOOTSEL |
                     RP_ROM_REBOOT2_FLAG_NO_RETURN_ON_SUCCESS,
                     10U, flags, gpio);
#endif

  osalSysHalt("bootrom return");
  __builtin_unreachable();
}

#if defined(RP2040)
bool rpRomGetMemApiX(rp_rom_mem_api_t *apip) {
  uintptr_t table[] = {
    RP_ROM_FUNC_MEMCPY,
    RP_ROM_FUNC_MEMCPY44,
    RP_ROM_FUNC_MEMSET,
    RP_ROM_FUNC_MEMSET4
  };
  bool ok;

  osalDbgCheck(apip != NULL);

  *apip = (rp_rom_mem_api_t){0};
  ok = rpRomFuncsLookupX(table, 4U);

  apip->memcpy = (rp_rom_memcpy_fn_t)table[0];
  apip->memcpy44 = (rp_rom_memcpy44_fn_t)table[1];
  apip->memset = (rp_rom_memset_fn_t)table[2];
  apip->memset4 = (rp_rom_memset4_fn_t)table[3];

  return ok;
}

bool rpRomGetBitApiX(rp_rom_bit_api_t *apip) {
  uintptr_t table[] = {
    RP_ROM_FUNC_CLZ32,
    RP_ROM_FUNC_CTZ32,
    RP_ROM_FUNC_POPCOUNT32,
    RP_ROM_FUNC_REVERSE32
  };
  bool ok;

  osalDbgCheck(apip != NULL);

  *apip = (rp_rom_bit_api_t){0};
  ok = rpRomFuncsLookupX(table, 4U);

  apip->clz32 = (rp_rom_clz32_fn_t)table[0];
  apip->ctz32 = (rp_rom_ctz32_fn_t)table[1];
  apip->popcount32 = (rp_rom_popcount32_fn_t)table[2];
  apip->reverse32 = (rp_rom_reverse32_fn_t)table[3];

  return ok;
}
#endif

/** @} */
