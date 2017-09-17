/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    SAMA5D2x/aic.h
 * @brief   SAMA AIC support macros and structures.
 *
 * @addtogroup COMMON_SAMA5D2x_AIC
 * @{
 */

#ifndef AIC_H
#define AIC_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   AIC unique redirect key.
 */
#define AIC_REDIR_KEY                       0x5B6C0E26U

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

/**
 * @brief   Acknowledge the current interrupt.
 */
#define aicAckInt() {                                                       \
  SAIC->AIC_EOICR = AIC_EOICR_ENDIT;                                        \
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void aicInit(void);
  void aicSetSourcePriority(uint32_t source, uint8_t priority);
  void aicSetSourceHandler(uint32_t source, bool (*handler)(void));
  void aicSetSpuriousHandler(bool (*handler)(void));
  void aicEnableInt(uint32_t source);
  void aicDisableInt(uint32_t source);
  void aicClearInt(uint32_t source);
  void aicSetInt(uint32_t source);
#ifdef __cplusplus
}
#endif

#endif /* AIC_H */

/** @} */
