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
 * @file    SAMA5D2x/sama_matrix.h
 * @brief   SAMA MATRIX support macros and structures.
 *
 * @addtogroup SAMA5D2x_MATRIX
 * @{
 */

#ifndef SAMA_MATRIX_H
#define SAMA_MATRIX_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define LOWER_AREA_SECURABLE     0x0u
#define UPPER_AREA_SECURABLE     0x1u

#define SECURE_READ              0x0u
#define SECURE_WRITE             0x0u
#define NOT_SECURE_READ          0x1u
#define NOT_SECURE_WRITE         0x1u

#define SECURE_PER               FALSE
#define NOT_SECURE_PER           TRUE

#define NO_DEFAULT_MASTER        0x0u
#define LAST_DEFAULT_MASTER      0x1u
#define FIXED_DEFAULT_MASTER     0x2u

#define REGION_0                 (0x1u << 0)
#define REGION_1                 (0x1u << 1)
#define REGION_2                 (0x1u << 2)
#define REGION_3                 (0x1u << 3)
#define REGION_4                 (0x1u << 4)
#define REGION_5                 (0x1u << 5)
#define REGION_6                 (0x1u << 6)
#define REGION_7                 (0x1u << 7)

#define MATRIX_AREA_SIZE_4K      0x0u
#define MATRIX_AREA_SIZE_8K      0x1u
#define MATRIX_AREA_SIZE_16K     0x2u
#define MATRIX_AREA_SIZE_32K     0x3u
#define MATRIX_AREA_SIZE_64K     0x4u
#define MATRIX_AREA_SIZE_128K    0x5u
#define MATRIX_AREA_SIZE_256K    0x6u
#define MATRIX_AREA_SIZE_512K    0x7u
#define MATRIX_AREA_SIZE_1M      0x8u
#define MATRIX_AREA_SIZE_2M      0x9u
#define MATRIX_AREA_SIZE_4M      0xAu
#define MATRIX_AREA_SIZE_8M      0xBu
#define MATRIX_AREA_SIZE_16M     0xCu
#define MATRIX_AREA_SIZE_32M     0xDu
#define MATRIX_AREA_SIZE_64M     0xEu
#define MATRIX_AREA_SIZE_128M    0xFu

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
 bool mtxConfigPeriphSecurity(Matrix *mtxp, uint8_t id, bool mode);
 void mtxConfigDefaultMaster(Matrix *mtxp, uint8_t slaveID,
                             uint8_t type, uint8_t masterID);
 void mtxConfigSlaveSec(Matrix *mtxp, uint8_t slaveID,
                        uint8_t selMask, uint8_t readMask,
                        uint8_t writeMask);
 void mtxSetSlaveSplitAddr(Matrix *mtxp, uint8_t slaveID,
                           uint8_t area, uint8_t mask);
 void mtxSetSlaveRegionSize(Matrix *mtxp, uint8_t slaveID,
                            uint8_t areaSize, uint8_t mask);
 void mtxRemapRom(void);
 void mtxRemapRam(void);
#ifdef __cplusplus
}
#endif

#endif /* SAMA_MATRIX_H */

/** @} */
