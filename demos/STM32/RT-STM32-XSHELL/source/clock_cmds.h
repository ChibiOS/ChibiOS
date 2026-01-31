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

#if !defined(CLK_POINT_NAMES)
#error "HAL port does not export required macro CLK_POINT_NAMES"
#endif

#ifdef __cplusplus
extern "C" {
#endif
#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT)
  void cmd_clock(xshell_t *xshp, int argc, char *argv[], char *envp[]);
#endif
 void cmd_clocks(xshell_t *xshp, int argc, char *argv[], char *envp[]);
#ifdef __cplusplus
}
#endif
