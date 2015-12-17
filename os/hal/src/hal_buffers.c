/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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
 * @file    hal_buffers.c
 * @brief   I/O Buffers code.
 *
 * @addtogroup HAL_BUFFERS
 * @{
 */

#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an I/O buffer object.
 *
 * @param[out] iobp     pointer to the @p input_double_buffer_t object
 * @param[in] bp        pointer to a memory area allocated as buffer
 * @param[in] size      size of the buffers
 *
 * @init
 */
void iobInit(io_buffer_t *iobp, uint8_t *bp, size_t size) {

  iobp->buffer = bp;
  iobp->limit  = bp;
  iobp->top    = bp + size;
}

/**
 * @brief   Initializes an input double buffer object.
 *
 * @param[out] idbp     pointer to the @p input_double_buffer_t object
 * @param[in] b1p       pointer to a memory area allocated as buffer 1
 * @param[in] b2p       pointer to a memory area allocated as buffer 2
 * @param[in] size      size of the buffers
 *
 * @init
 */
void idbObjectInit(input_double_buffer_t *idbp,
                   uint8_t *b1p, uint8_t *b2p, size_t size,
                   dbnotify_t infy, void *link) {

  iobInit(&idbp->buffers[0], b1p, size);
  iobInit(&idbp->buffers[1], b2p, size);
  idbp->counter = 0;
  idbp->brdptr  = &idbp->buffers[0];
  idbp->bwrptr  = &idbp->buffers[0];
  idbp->notify  = infy;
  idbp->link    = link;
}

/** @} */
