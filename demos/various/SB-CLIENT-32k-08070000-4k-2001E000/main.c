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

#include <stdio.h>

#include "sbuser.h"

void Vector00(void) {

  printf("#1 Hello World (%u)!!\r\n", (unsigned)0);
}

/*
 * Application entry point.
 */
int main(void) {

#if 0
  /* Test for exception on interrupt.*/
  asm volatile ("mov r0, #64");
  asm volatile ("mov sp, r0");
  while (true) {
  }
#endif
  __sb_vrq_seten(1);
//  __sb_vrq_setwt(1);
  sbSetAlarm(sbTimeMS2I(300), true);

  while (true) {
    msg_t msg = sbMsgWait();
//    printf("#1 Hello World (%u)!!\r\n", (unsigned)msg);
//    sbFileWrite(1U, (const uint8_t *)"#1 Hello World!!\r\n", 15U);
    sbMsgReply(msg);
//    sbSleepMilliseconds(500);
  }
}
