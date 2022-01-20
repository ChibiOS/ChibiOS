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
#include "sbhdr.h"
#include "syscalls.h"

extern int __callelf(sb_header_t *sbhp, int argc, char *argv[], char *envp[]);
extern int __returnelf(void);

int runelf(int argc, char *argv[], char *envp[]) {
  uint8_t *buf, *bufend;
  sb_header_t *sbhp;
  msg_t ret;

  /* Boundaries of available RAM in the sandbox.*/
  buf = sbrk(0);
  bufend = __sb_parameters.heap_end;

  /* Aligning the start address.*/
  buf = (uint8_t *)((((uint32_t)buf - 1U) | 3U) + 1U);
  if (buf >= bufend) {
    errno = ENOMEM;
    return -1;
  }

  /* Loading the specified file.*/
  ret = sbLoadElf(argv[0], buf, (size_t)(bufend - buf));
  if (CH_RET_IS_ERROR(ret)) {
    errno =  CH_DECODE_ERROR(ret);
    return -1;
  }

  /* Pointer to the executable header.*/
  sbhp = (sb_header_t *)buf;

  /* Checking header.*/
  if ((sbhp->hdr_magic1 != SB_HDR_MAGIC1) ||
      (sbhp->hdr_magic2 != SB_HDR_MAGIC2) ||
      (sbhp->hdr_size != sizeof (sb_header_t))) {
    errno =  ENOEXEC;
    return -1;
  }

  /* Setting up the exit vector for the loaded elf file.*/
  sbhp->hdr_exit = (uint32_t)__returnelf;
  sbhp->user[0]  = (uint32_t)bufend;
  return __callelf(sbhp, argc, argv, envp);
}
