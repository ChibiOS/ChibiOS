/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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

#include <stdlib.h>

#include "ch.h"
#include "hal.h"
#include "console.h"
#include "hal_mfs.h"
#include "mfs_test_root.h"

#define SIM_MFS_SECTOR_SIZE                 SIM_EFL_SECTOR_SIZE
#define SIM_MFS_BANK_SECTORS                8U
#define SIM_MFS_BANK_SIZE                   (SIM_MFS_SECTOR_SIZE * SIM_MFS_BANK_SECTORS)

const MFSConfig mfscfg1 = {
  .flashp           = (BaseFlash *)&EFLD1,
  .erased           = 0xFFFFFFFFU,
  .bank_size        = SIM_MFS_BANK_SIZE,
  .bank0_start      = 0U,
  .bank0_sectors    = SIM_MFS_BANK_SECTORS,
  .bank1_start      = SIM_MFS_BANK_SECTORS,
  .bank1_sectors    = SIM_MFS_BANK_SECTORS
};

/*
 * Simulator main.
 */
int main(int argc, char *argv[]) {

  (void)argc;
  (void)argv;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  conInit();
  chSysInit();

  eflStart(&EFLD1, NULL);

  test_execute((BaseSequentialStream *)&CD1, &mfs_test_suite);

  exit(chtest.global_fail ? 1 : 0);
}
