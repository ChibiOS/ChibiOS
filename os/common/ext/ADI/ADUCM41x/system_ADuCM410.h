/*!
 *****************************************************************************
 * @file:  SpiLib.h 
 * @brief: header of SPI interface
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2013-2018 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.
By using this software you agree to the terms of the associated
Analog Devices Software License Agreement.
 ******************************************************************************/
#ifndef SYSTEM_ADUCM410_H
#define SYSTEM_ADUCM410_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "ADuCM410.h"

extern void SystemInit(void);

/**
  \brief  Update SystemCoreClock variable.

   Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */

#ifndef __ICCARM__
extern const uint32_t page0_checksum __attribute__((weak,section("PAGE0_CHECKSUM"))); 
#else
#pragma location=".PAGE0_CHECKSUM"
extern __root const uint32_t page0_checksum; 
#endif


#ifdef __cplusplus
}
#endif

#endif   //#SYSTEM_ADUCM410_H
