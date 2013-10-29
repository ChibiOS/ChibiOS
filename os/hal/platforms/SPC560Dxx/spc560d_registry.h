/*
    SPC5 HAL - Copyright (C) 2013 STMicroelectronics

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
 * @file    SPC560Dxx/spc560d_registry.h
 * @brief   SPC560Dxx capabilities registry.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _SPC560D_REGISTRY_H_
#define _SPC560D_REGISTRY_H_

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    SPC560Dxx capabilities
 * @{
 */
/* DSPI attribures.*/
#define SPC5_HAS_DSPI0                      TRUE
#define SPC5_HAS_DSPI1                      TRUE
#define SPC5_HAS_DSPI2                      FALSE
#define SPC5_HAS_DSPI3                      FALSE
#define SPC5_HAS_DSPI4                      FALSE
#define SPC5_DSPI_FIFO_DEPTH                4
#define SPC5_DSPI0_PCTL                     4
#define SPC5_DSPI1_PCTL                     5
#define SPC5_DSPI0_TX1_DMA_DEV_ID           1
#define SPC5_DSPI0_TX2_DMA_DEV_ID           0
#define SPC5_DSPI0_RX_DMA_DEV_ID            2
#define SPC5_DSPI1_TX1_DMA_DEV_ID           3
#define SPC5_DSPI1_TX2_DMA_DEV_ID           0
#define SPC5_DSPI1_RX_DMA_DEV_ID            4
#define SPC5_DSPI0_TFFF_HANDLER             vector76
#define SPC5_DSPI0_TFFF_NUMBER              76
#define SPC5_DSPI1_TFFF_HANDLER             vector96
#define SPC5_DSPI1_TFFF_NUMBER              96
#define SPC5_DSPI0_ENABLE_CLOCK()                                           \
  halSPCSetPeripheralClockMode(SPC5_DSPI0_PCTL, SPC5_SPI_DSPI0_START_PCTL)
#define SPC5_DSPI0_DISABLE_CLOCK()                                          \
  halSPCSetPeripheralClockMode(SPC5_DSPI0_PCTL, SPC5_SPI_DSPI0_STOP_PCTL)
#define SPC5_DSPI1_ENABLE_CLOCK()                                           \
  halSPCSetPeripheralClockMode(SPC5_DSPI1_PCTL, SPC5_SPI_DSPI1_START_PCTL)
#define SPC5_DSPI1_DISABLE_CLOCK()                                          \
  halSPCSetPeripheralClockMode(SPC5_DSPI1_PCTL, SPC5_SPI_DSPI1_STOP_PCTL)

/* eDMA attributes.*/
#define SPC5_HAS_EDMA                       TRUE
#define SPC5_EDMA_NCHANNELS                 16
#define SPC5_EDMA_HAS_MUX                   TRUE
#define SPC5_EDMA_MUX_PCTL                  23

/* LINFlex attributes.*/
#define SPC5_HAS_LINFLEX0                   TRUE
#define SPC5_LINFLEX0_PCTL                  48
#define SPC5_LINFLEX0_RXI_HANDLER           vector79
#define SPC5_LINFLEX0_TXI_HANDLER           vector80
#define SPC5_LINFLEX0_ERR_HANDLER           vector81
#define SPC5_LINFLEX0_RXI_NUMBER            79
#define SPC5_LINFLEX0_TXI_NUMBER            80
#define SPC5_LINFLEX0_ERR_NUMBER            81
#define SPC5_LINFLEX0_CLK                   (halSPCGetSystemClock() /       \
                                             SPC5_PERIPHERAL1_CLK_DIV_VALUE)

#define SPC5_HAS_LINFLEX1                   TRUE
#define SPC5_LINFLEX1_PCTL                  49
#define SPC5_LINFLEX1_RXI_HANDLER           vector99
#define SPC5_LINFLEX1_TXI_HANDLER           vector100
#define SPC5_LINFLEX1_ERR_HANDLER           vector101
#define SPC5_LINFLEX1_RXI_NUMBER            99
#define SPC5_LINFLEX1_TXI_NUMBER            100
#define SPC5_LINFLEX1_ERR_NUMBER            101
#define SPC5_LINFLEX1_CLK                   (halSPCGetSystemClock() /       \
                                             SPC5_PERIPHERAL1_CLK_DIV_VALUE)

#define SPC5_HAS_LINFLEX2                   TRUE
#define SPC5_LINFLEX2_PCTL                  50
#define SPC5_LINFLEX2_RXI_HANDLER           vector119
#define SPC5_LINFLEX2_TXI_HANDLER           vector120
#define SPC5_LINFLEX2_ERR_HANDLER           vector121
#define SPC5_LINFLEX2_RXI_NUMBER            119
#define SPC5_LINFLEX2_TXI_NUMBER            120
#define SPC5_LINFLEX2_ERR_NUMBER            121
#define SPC5_LINFLEX2_CLK                   (halSPCGetSystemClock() /       \
                                             SPC5_PERIPHERAL1_CLK_DIV_VALUE)

#define SPC5_HAS_LINFLEX3                   FALSE

/* SIUL attributes.*/
#define SPC5_HAS_SIUL                       TRUE
#define SPC5_SIUL_PCTL                      68
#define SPC5_SIUL_NUM_PORTS                 8
#define SPC5_SIUL_NUM_PCRS                  77
#define SPC5_SIUL_NUM_PADSELS               63
#define SPC5_SIUL_SYSTEM_PINS               32,33

/* FlexCAN attributes.*/
#define SPC5_HAS_FLEXCAN0                                   TRUE
#define SPC5_FLEXCAN0_PCTL                                  16
#define SPC5_FLEXCAN0_MB                                    32
#define SPC5_FLEXCAN0_SHARED_IRQ                            TRUE
#define SPC5_FLEXCAN0_FLEXCAN_ESR_ERR_INT_HANDLER           vector65
#define SPC5_FLEXCAN0_FLEXCAN_ESR_BOFF_HANDLER              vector66
#define SPC5_FLEXCAN0_FLEXCAN_BUF_00_03_HANDLER             vector68
#define SPC5_FLEXCAN0_FLEXCAN_BUF_04_07_HANDLER             vector69
#define SPC5_FLEXCAN0_FLEXCAN_BUF_08_11_HANDLER             vector70
#define SPC5_FLEXCAN0_FLEXCAN_BUF_12_15_HANDLER             vector71
#define SPC5_FLEXCAN0_FLEXCAN_BUF_16_31_HANDLER             vector72
#define SPC5_FLEXCAN0_FLEXCAN_ESR_ERR_INT_NUMBER            65
#define SPC5_FLEXCAN0_FLEXCAN_ESR_BOFF_NUMBER               66
#define SPC5_FLEXCAN0_FLEXCAN_BUF_00_03_NUMBER              68
#define SPC5_FLEXCAN0_FLEXCAN_BUF_04_07_NUMBER              69
#define SPC5_FLEXCAN0_FLEXCAN_BUF_08_11_NUMBER              70
#define SPC5_FLEXCAN0_FLEXCAN_BUF_12_15_NUMBER              71
#define SPC5_FLEXCAN0_FLEXCAN_BUF_16_31_NUMBER              72
#define SPC5_FLEXCAN0_ENABLE_CLOCK()                        halSPCSetPeripheralClockMode(SPC5_FLEXCAN0_PCTL, SPC5_CAN_FLEXCAN0_START_PCTL);
#define SPC5_FLEXCAN0_DISABLE_CLOCK()                       halSPCSetPeripheralClockMode(SPC5_FLEXCAN0_PCTL, SPC5_CAN_FLEXCAN0_STOP_PCTL);
/** @} */

#endif /* _SPC560D_REGISTRY_H_ */

/** @} */
