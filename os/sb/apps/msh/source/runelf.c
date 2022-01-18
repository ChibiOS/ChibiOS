/*
    ChibiOS - Copyright (C) 2006..2022 Giovanni Di Sirio

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

#include <unistd.h>

#include "sbuser.h"
#include "syscalls.h"

int runelf(const char *fname, char *argv[], char *envp[]) {
  uint8_t *buf, *bufend;
  size_t size;
  msg_t ret;

  buf = sbrk(0);
  bufend = __sb_parameters.heap_end;

  buf = (uint8_t *)((((uint32_t)buf - 1U) | 3U) + 1U);
  if (buf >= bufend) {
    return ENOMEM;
  }

  size = (size_t)(bufend - buf);
  ret = sbLoadElf(fname, bufend, size);
  if (CH_RET_IS_ERROR(ret)) {
    return CH_DECODE_ERROR(ret);
  }
}
