/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @mainpage Test Suite Specification
 * Test suite for ChibiOS/HAL MFS. The purpose of this suite is to
 * perform unit tests on the MFS module and to converge to 100% code
 * coverage through successive improvements.
 *
 * <h2>Test Sequences</h2>
 * - @subpage mfs_test_sequence_001
 * - @subpage mfs_test_sequence_002
 * .
 */

/**
 * @file    mfs_test_root.c
 * @brief   Test Suite root structures code.
 */

#include "hal.h"
#include "mfs_test_root.h"

#if !defined(__DOXYGEN__)

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Array of test sequences.
 */
const testsequence_t * const mfs_test_suite_array[] = {
  &mfs_test_sequence_001,
  &mfs_test_sequence_002,
  NULL
};

/**
 * @brief   Test suite root structure.
 */
const testsuite_t mfs_test_suite = {
  "ChibiOS/HAL MFS Test Suite",
  mfs_test_suite_array
};

/*===========================================================================*/
/* Shared code.                                                              */
/*===========================================================================*/

#include "hal_mfs.h"

MFSDriver mfs1;
uint8_t mfs_buffer[512];

void test_print_mfs_info(void) {

}

flash_error_t bank_erase(mfs_bank_t bank) {
  flash_sector_t sector, n;

  if (bank == MFS_BANK_0) {
    sector = mfscfg1.bank0_start;
    n      = mfscfg1.bank0_sectors;
  }
  else {
    sector = mfscfg1.bank1_start;
    n      = mfscfg1.bank1_sectors;
  }

  while (n--) {
    flash_error_t ferr;

    ferr = flashStartEraseSector(mfscfg1.flashp, sector);
    if (ferr != FLASH_NO_ERROR)
      return ferr;
    ferr = flashWaitErase(mfscfg1.flashp);
    if (ferr != FLASH_NO_ERROR)
      return ferr;
    sector++;
  }
  return FLASH_NO_ERROR;
}

flash_error_t bank_verify_erased(mfs_bank_t bank) {
  flash_sector_t sector, n;

  if (bank == MFS_BANK_0) {
    sector = mfscfg1.bank0_start;
    n      = mfscfg1.bank0_sectors;
  }
  else {
    sector = mfscfg1.bank1_start;
    n      = mfscfg1.bank1_sectors;
  }

  while (n--) {
    flash_error_t ferr;

    ferr = flashVerifyErase(mfscfg1.flashp, sector);
    if (ferr != FLASH_NO_ERROR)
      return ferr;
    sector++;
  }
  return FLASH_NO_ERROR;
}

#endif /* !defined(__DOXYGEN__) */
