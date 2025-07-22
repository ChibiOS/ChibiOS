/*
    ChibiOS - Copyright (C) 2006..2021 Giovanni Di Sirio

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
 * @file    RP2350/rp_fifo.h
 * @brief   RP2350 FIFO handler header.
 *
 * @addtogroup RP2350_FIFO
 * @{
 */

#ifndef RP_FIFO_H
#define RP_FIFO_H

#include "hardware/structs/sio.h" // for sio_hw


/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* EInline functions.                                                        */
/*===========================================================================*/

__STATIC_INLINE bool fifoIsReadNotEmpty(void) {
  return (bool)((sio_hw->fifo_st & SIO_FIFO_ST_VLD_BITS) != 0U);
}

__STATIC_INLINE bool fifoIsWriteNotFull(void) {

  //return (bool)((SIO->FIFO_ST & SIO_FIFO_ST_RDY) != 0U);
  return (bool)((sio_hw->fifo_st & SIO_FIFO_ST_RDY_BITS) != 0U); 
}

__STATIC_INLINE void fifoFlushRead(void) {

  while (fifoIsReadNotEmpty()) {
    (void)sio_hw->fifo_rd; 
  }

  /* In case the other core is in WFE.*/
  __SEV();
}

__STATIC_INLINE void fifoBlockingWrite(uint32_t data) {

  /* Waiting for space into the FIFO.*/
  while (!fifoIsWriteNotFull()) {
    __WFE();
  }

  sio_hw->fifo_wr = data; 

  /* In case the other core is in WFE, signaling data available.*/
  __SEV();
}

__STATIC_INLINE uint32_t fifoBlockingRead(void) {
  uint32_t data;

  /* Waiting for data to appear.*/
  while (!fifoIsReadNotEmpty()) {
    __WFE();
  }

  data = sio_hw->fifo_rd; 

  /* In case the other core is in WFE, signaling space available.*/
  __SEV();

  return data;
}

#endif /* RP_FIFO_H */

/** @} */
