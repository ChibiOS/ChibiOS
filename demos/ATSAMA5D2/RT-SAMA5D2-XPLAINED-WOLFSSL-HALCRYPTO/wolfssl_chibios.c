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
 * **** This file incorporates work covered by the following copyright and ****
 * **** permission notice:                                                 ****
 *
 * Copyright (C) 2006-2017 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 *
 */

#include "ch.h"
#include "wolfssl_chibios.h"
#include <string.h>



#ifndef ST2S
#   define ST2S(n) (((n) + CH_CFG_ST_FREQUENCY - 1UL) / CH_CFG_ST_FREQUENCY)
#endif

#ifndef ST2MS
#define ST2MS(n) (((n) * 1000UL + CH_CFG_ST_FREQUENCY - 1UL) / CH_CFG_ST_FREQUENCY)
#endif


word32 LowResTimer(void)
{
    systime_t t = chVTGetSystemTimeX();
    return ST2S(t);
}

uint32_t TimeNowInMilliseconds(void)
{
    systime_t t = chVTGetSystemTimeX();
    return ST2MS(t);
}

void *chHeapRealloc (void *addr, uint32_t size)
{
    union heap_header *hp;
    uint32_t prev_size, new_size;

    void *ptr;

    if(addr == NULL) {
        return chHeapAlloc(NULL, size);
    }

    /* previous allocated segment is preceded by an heap_header */
    hp = addr - sizeof(union heap_header);
    prev_size = hp->used.size; /* size is always multiple of 8 */

    /* check new size memory alignment */
    if(size % 8 == 0) {
        new_size = size;
    }
    else {
        new_size = ((int) (size / 8)) * 8 + 8;
    }

    if(prev_size >= new_size) {
        return addr;
    }

    ptr = chHeapAlloc(NULL, size);
    if(ptr == NULL) {
        return NULL;
    }

    memcpy(ptr, addr, prev_size);

    chHeapFree(addr);

    return ptr;
}

void *chibios_alloc(void *heap, int size)
{
    return chHeapAlloc(heap, size);
}

void chibios_free(void *ptr)
{
    if (ptr)
        chHeapFree(ptr);
}

