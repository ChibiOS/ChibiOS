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
#include <dirent.h>

#include "sbuser.h"

/*
 * Application entry point.
 */
int main(void) {
  unsigned i = 1U;
  DIR *dirp;

  sbSleepMilliseconds(2000);
  dirp = opendir("/");
  if (dirp != NULL) {
    struct dirent *dep;

    while ((dep = readdir(dirp)) != NULL) {
      printf("(%u) %s\r\n", dep->d_type, dep->d_name);
    }

    closedir(dirp);
  }
  while (true) {
    printf("#1 Hello World (%u)!!\r\n", i++);
    sbSleepMilliseconds(500);
  }
}
