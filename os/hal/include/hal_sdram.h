/*
    ChibiOS/HAL - Copyright (C) 2014 Uladzimir Pylinsky aka barthess

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
   SDRAM routines added by Nick Klimov aka progfin.
 */

/**
 * @file    hal_sdram.h
 * @brief   SDRAM Driver subsystem low level driver header.
 *
 * @addtogroup SDRAM
 * @{
 */

#ifndef HAL_SDRAM_H_
#define HAL_SDRAM_H_

#if (HAL_USE_SDRAM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 *  FMC SDRAM Mode definition register defines
 */
#define FMC_SDCMR_MRD_BURST_LENGTH_1             ((uint16_t)0x0000)
#define FMC_SDCMR_MRD_BURST_LENGTH_2             ((uint16_t)0x0001)
#define FMC_SDCMR_MRD_BURST_LENGTH_4             ((uint16_t)0x0002)
#define FMC_SDCMR_MRD_BURST_LENGTH_8             ((uint16_t)0x0004)
#define FMC_SDCMR_MRD_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define FMC_SDCMR_MRD_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define FMC_SDCMR_MRD_CAS_LATENCY_2              ((uint16_t)0x0020)
#define FMC_SDCMR_MRD_CAS_LATENCY_3              ((uint16_t)0x0030)
#define FMC_SDCMR_MRD_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define FMC_SDCMR_MRD_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define FMC_SDCMR_MRD_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

/*
 * FMC_ReadPipe_Delay
 */
#define FMC_ReadPipe_Delay_0               ((uint32_t)0x00000000)
#define FMC_ReadPipe_Delay_1               ((uint32_t)0x00002000)
#define FMC_ReadPipe_Delay_2               ((uint32_t)0x00004000)
#define FMC_ReadPipe_Delay_Mask            ((uint32_t)0x00006000)

/*
 * FMC_Read_Burst
 */
#define FMC_Read_Burst_Disable             ((uint32_t)0x00000000)
#define FMC_Read_Burst_Enable              ((uint32_t)0x00001000)
#define FMC_Read_Burst_Mask                ((uint32_t)0x00001000)

/*
 * FMC_SDClock_Period
 */
#define FMC_SDClock_Disable                ((uint32_t)0x00000000)
#define FMC_SDClock_Period_2               ((uint32_t)0x00000800)
#define FMC_SDClock_Period_3               ((uint32_t)0x00000C00)
#define FMC_SDClock_Period_Mask            ((uint32_t)0x00000C00)

/*
 * FMC_ColumnBits_Number
 */
#define FMC_ColumnBits_Number_8b           ((uint32_t)0x00000000)
#define FMC_ColumnBits_Number_9b           ((uint32_t)0x00000001)
#define FMC_ColumnBits_Number_10b          ((uint32_t)0x00000002)
#define FMC_ColumnBits_Number_11b          ((uint32_t)0x00000003)

/*
 * FMC_RowBits_Number
 */
#define FMC_RowBits_Number_11b             ((uint32_t)0x00000000)
#define FMC_RowBits_Number_12b             ((uint32_t)0x00000004)
#define FMC_RowBits_Number_13b             ((uint32_t)0x00000008)

/*
 * FMC_SDMemory_Data_Width
 */
#define FMC_SDMemory_Width_8b                ((uint32_t)0x00000000)
#define FMC_SDMemory_Width_16b               ((uint32_t)0x00000010)
#define FMC_SDMemory_Width_32b               ((uint32_t)0x00000020)

/*
 * FMC_InternalBank_Number
 */
#define FMC_InternalBank_Number_2          ((uint32_t)0x00000000)
#define FMC_InternalBank_Number_4          ((uint32_t)0x00000040)

/*
 * FMC_CAS_Latency
 */
#define FMC_CAS_Latency_1                  ((uint32_t)0x00000080)
#define FMC_CAS_Latency_2                  ((uint32_t)0x00000100)
#define FMC_CAS_Latency_3                  ((uint32_t)0x00000180)

/*
 * FMC_Write_Protection
 */
#define FMC_Write_Protection_Disable       ((uint32_t)0x00000000)
#define FMC_Write_Protection_Enable        ((uint32_t)0x00000200)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/
/**
 * @name    Configuration options
 * @{
 */

/**
 * @brief   SDRAM driver enable switch.
 * @details If set to @p TRUE the support for SDRAM1 is included.
 */
#if !defined(STM32_SDRAM_USE_SDRAM1) || defined(__DOXYGEN__)
#define STM32_SDRAM_USE_SDRAM1                  FALSE
#else
#define STM32_SDRAM1_MAP_BASE                   FSMC_Bank5_MAP_BASE
#endif

/**
 * @brief   SDRAM driver enable switch.
 * @details If set to @p TRUE the support for SDRAM2 is included.
 */
#if !defined(STM32_SDRAM_USE_SDRAM2) || defined(__DOXYGEN__)
#define STM32_SDRAM_USE_SDRAM2                  FALSE
#else
#define STM32_SDRAM2_MAP_BASE                   FSMC_Bank6_MAP_BASE
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !STM32_SDRAM_USE_SDRAM1 && !STM32_SDRAM_USE_SDRAM2
#error "SDRAM driver activated but no SDRAM peripheral assigned"
#endif

#if (STM32_SDRAM_USE_SDRAM1 || STM32_SDRAM_USE_SDRAM2) && !STM32_HAS_FSMC
#error "FMC not present in the selected device"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/
/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  SDRAM_UNINIT = 0,                   /**< Not initialized.                */
  SDRAM_STOP = 1,                     /**< Stopped.                        */
  SDRAM_READY = 2,                    /**< Ready.                          */
} sdramstate_t;

/**
 * @brief   Type of a structure representing an SDRAM driver.
 */
typedef struct SDRAMDriver SDRAMDriver;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief     SDRAM control register.
   * @note      Its value will be used for both banks.
   */
  uint32_t      sdcr;

  /**
   * @brief     SDRAM timing register.
   * @note      Its value will be used for both banks.
   */
  uint32_t      sdtr;

  /**
   * @brief     SDRAM command mode register.
   * @note      Only its MRD and NRFS bits will be used.
   */
  uint32_t      sdcmr;

  /**
   * @brief     SDRAM refresh timer register.
   * @note      Only its COUNT bits will be used.
   */
  uint32_t      sdrtr;
} SDRAMConfig;

/**
 * @brief   Structure representing an SDRAM driver.
 */
struct SDRAMDriver {
  /**
   * @brief     Driver state.
   */
  sdramstate_t              state;
  /**
   * @brief     Pointer to the FMC SDRAM registers block.
   */
  FSMC_SDRAM_TypeDef        *sdram;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern SDRAMDriver SDRAMD1;

#ifdef __cplusplus
extern "C" {
#endif
  void sdramInit(void);
  void sdramObjectInit(SDRAMDriver *sdramp);
  void sdramStart(SDRAMDriver *sdramp, const SDRAMConfig *cfgp);
  void sdramStop(SDRAMDriver *sdramp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SDRAM */

#endif /* HAL_SDRAM_H_ */

/** @} */
