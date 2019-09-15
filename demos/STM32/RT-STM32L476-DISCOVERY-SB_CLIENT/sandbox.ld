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

/*
 * Sandbox memory setup.
 */
MEMORY
{
    flash0  : org = 0x080F8000, len = 32k
    ram0    : org = 0x20017000, len = 4k
}

/* Symbols required by the sandbox startup file.*/
__r1_start__            = ORIGIN(flash0);
__r1_size__             = LENGTH(flash0);
__r1_end__              = __r1_start__ + __r1_size__;
__r2_start__            = ORIGIN(ram0);
__r2_size__             = LENGTH(ram0);
__r2_end__              = __r2_start__ + __r2_size__;

/* Flash region to be used for exception vectors.*/
REGION_ALIAS("CODE_SPACE", flash0);

/* RAM region to be used for data.*/
REGION_ALIAS("DATA_SPACE", ram0);

/* Generic rules inclusion.*/
INCLUDE rules.ld
