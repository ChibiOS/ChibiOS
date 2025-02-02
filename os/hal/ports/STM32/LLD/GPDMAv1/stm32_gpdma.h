/*
    ChibiOS - Copyright (C) 2006..2023 Giovanni Di Sirio

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
 * @file    DMAv1/stm32_gpdma.h
 * @brief   GPDMA helper driver header.
 *
 * @addtogroup STM32_GPDMA
 * @{
 */

#ifndef STM32_GPDMA_H
#define STM32_GPDMA_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    DMA registers helpers
 * @{
 */
#define STM32_GPDMA_CLBAR_LBA_POS       DMA_CLBAR_LBA_Pos
#define STM32_GPDMA_CLBAR_LBA_MASK      (0xFFFFU << STM32_GPDMA_CLBAR_LBA_POS)
#define STM32_GPDMA_CLBAR_LBA(n)        ((n) << STM32_GPDMA_CLBAR_LBA_POS)

#define STM32_GPDMA_CFCR_TOF            DMA_CFCR_TOF
#define STM32_GPDMA_CFCR_SUSPF          DMA_CFCR_SUSPF
#define STM32_GPDMA_CFCR_USEF           DMA_CFCR_USEF
#define STM32_GPDMA_CFCR_ULEF           DMA_CFCR_ULEF
#define STM32_GPDMA_CFCR_DTEF           DMA_CFCR_DTEF
#define STM32_GPDMA_CFCR_HTF            DMA_CFCR_HTF
#define STM32_GPDMA_CFCR_TCF            DMA_CFCR_TCF
#define STM32_GPDMA_CFCR_ALL_FLAGS      (DMA_CFCR_TOF  | DMA_CFCR_SUSPF |   \
                                         DMA_CFCR_USEF | DMA_CFCR_ULEF  |   \
                                         DMA_CFCR_DTEF | DMA_CFCR_HTF   |   \
                                         DMA_CFCR_TCF)

#define STM32_GPDMA_CSR_FIFOL_POS       DMA_CSR_FIFOL_Pos
#define STM32_GPDMA_CSR_FIFOL_MASK      (0x7FU << STM32_GPDMA_CSR_FIFOL_POS)
#define STM32_GPDMA_CSR_FIFOL(n)        ((n) << STM32_GPDMA_CSR_FIFOL_POS)
#define STM32_GPDMA_CSR_TOF             DMA_CSR_TOF
#define STM32_GPDMA_CSR_SUSPF           DMA_CSR_SUSPF
#define STM32_GPDMA_CSR_USEF            DMA_CSR_USEF
#define STM32_GPDMA_CSR_ULEF            DMA_CSR_ULEF
#define STM32_GPDMA_CSR_DTEF            DMA_CSR_DTEF
#define STM32_GPDMA_CSR_HTF             DMA_CSR_HTF
#define STM32_GPDMA_CSR_TCF             DMA_CSR_TCF
#define STM32_GPDMA_CSR_IDLEF           DMA_CSR_IDLEF
#define STM32_GPDMA_CSR_ERRORS          (DMA_CSR_TOF   |  DMA_CSR_USEF |    \
                                         DMA_CSR_ULEF  |  DMA_CSR_DTEF)
#define STM32_GPDMA_CSR_ALL_FLAGS       (DMA_CSR_TOF   | DMA_CSR_SUSPF |    \
                                         DMA_CSR_USEF  | DMA_CSR_ULEF  |    \
                                         DMA_CSR_DTEF  | DMA_CSR_HTF   |    \
                                         DMA_CSR_TCF)

#define STM32_GPDMA_CCR_PRIO_POS        DMA_CCR_PRIO_Pos
#define STM32_GPDMA_CCR_PRIO_MASK       (3U << STM32_GPDMA_CCR_PRIO_POS)
#define STM32_GPDMA_CCR_PRIO(n)         ((n) << STM32_GPDMA_CCR_PRIO_POS)
#define STM32_GPDMA_CCR_LAP_POS         DMA_CCR_LAP_Pos
#define STM32_GPDMA_CCR_LAP_MASK        (1U << STM32_GPDMA_CCR_LAP_POS)
#define STM32_GPDMA_CCR_LAP(n)          ((n) << STM32_GPDMA_CCR_LAP_POS)
#define STM32_GPDMA_CCR_LAP_MEM         STM32_GPDMA_CCR_LAP(STM32_GPDMA_MEMORY_PORT)
#define STM32_GPDMA_CCR_LAP_PER         STM32_GPDMA_CCR_LAP(STM32_GPDMA_PERIPHERAL_PORT)
#define STM32_GPDMA_CCR_LSM             DMA_CCR_LSM
#define STM32_GPDMA_CCR_TOIE            DMA_CCR_TOIE
#define STM32_GPDMA_CCR_SUSPIE          DMA_CCR_SUSPIE
#define STM32_GPDMA_CCR_USEIE           DMA_CCR_USEIE
#define STM32_GPDMA_CCR_ULEIE           DMA_CCR_ULEIE
#define STM32_GPDMA_CCR_DTEIE           DMA_CCR_DTEIE
#define STM32_GPDMA_CCR_HTIE            DMA_CCR_HTIE
#define STM32_GPDMA_CCR_TCIE            DMA_CCR_TCIE
#define STM32_GPDMA_CCR_SUSP            DMA_CCR_SUSP
#define STM32_GPDMA_CCR_RESET           DMA_CCR_RESET
#define STM32_GPDMA_CCR_EN              DMA_CCR_EN

#define STM32_GPDMA_CTR1_DSEC           DMA_CTR1_DSEC
#define STM32_GPDMA_CTR1_DAP_POS        DMA_CTR1_DAP_Pos
#define STM32_GPDMA_CTR1_DAP_MASK       (1U << STM32_GPDMA_CTR1_DAP_POS)
#define STM32_GPDMA_CTR1_DAP(n)         ((n) << STM32_GPDMA_CTR1_DAP_POS)
#define STM32_GPDMA_CTR1_DAP_MEM        STM32_GPDMA_CTR1_DAP(STM32_GPDMA_MEMORY_PORT)
#define STM32_GPDMA_CTR1_DAP_PER        STM32_GPDMA_CTR1_DAP(STM32_GPDMA_PERIPHERAL_PORT)
#define STM32_GPDMA_CTR1_DHX            DMA_CTR1_DHX
#define STM32_GPDMA_CTR1_DBX            DMA_CTR1_DBX
#define STM32_GPDMA_CTR1_DBL_POS        DMA_CTR1_DBL_1_Pos
#define STM32_GPDMA_CTR1_DBL_MASK       (0x3FU << STM32_GPDMA_CTR1_DBL_POS)
#define STM32_GPDMA_CTR1_DBL(n)         ((n) << STM32_GPDMA_CTR1_DBL_POS)
#define STM32_GPDMA_CTR1_DINC           DMA_CTR1_DINC
#define STM32_GPDMA_CTR1_DDW_LOG2_POS   DMA_CTR1_DDW_LOG2_Pos
#define STM32_GPDMA_CTR1_DDW_LOG2_MASK  (3U << STM32_GPDMA_CTR1_DDW_LOG2_POS)
#define STM32_GPDMA_CTR1_DDW_LOG2(n)    ((n) << STM32_GPDMA_CTR1_DDW_LOG2_POS)
#define STM32_GPDMA_CTR1_DDW_BYTE       STM32_GPDMA_CTR1_DDW_LOG2(0U)
#define STM32_GPDMA_CTR1_DDW_HALF       STM32_GPDMA_CTR1_DDW_LOG2(1U)
#define STM32_GPDMA_CTR1_DDW_WORD       STM32_GPDMA_CTR1_DDW_LOG2(2U)
#define STM32_GPDMA_CTR1_SSEC           DMA_CTR1_SSEC
#define STM32_GPDMA_CTR1_SAP_POS        DMA_CTR1_SAP_Pos
#define STM32_GPDMA_CTR1_SAP_MASK       (1U << STM32_GPDMA_CTR1_SAP_POS)
#define STM32_GPDMA_CTR1_SAP(n)         ((n) << STM32_GPDMA_CTR1_SAP_POS)
#define STM32_GPDMA_CTR1_SAP_MEM        STM32_GPDMA_CTR1_SAP(STM32_GPDMA_MEMORY_PORT)
#define STM32_GPDMA_CTR1_SAP_PER        STM32_GPDMA_CTR1_SAP(STM32_GPDMA_PERIPHERAL_PORT)
#define STM32_GPDMA_CTR1_SBX            DMA_CTR1_SBX
#define STM32_GPDMA_CTR1_PAM_POS        DMA_CTR1_PAM_Pos
#define STM32_GPDMA_CTR1_PAM_MASK       (3U << STM32_GPDMA_CTR1_PAM_POS)
#define STM32_GPDMA_CTR1_PAM(n)         ((n) << STM32_GPDMA_CTR1_PAM_POS)
#define STM32_GPDMA_CTR1_SBL_POS        DMA_CTR1_SBL_1_Pos
#define STM32_GPDMA_CTR1_SBL_MASK       (0x3FU << STM32_GPDMA_CTR1_SBL_POS)
#define STM32_GPDMA_CTR1_SBL(n)         ((n) << STM32_GPDMA_CTR1_SBL_POS)
#define STM32_GPDMA_CTR1_SINC           DMA_CTR1_SINC
#define STM32_GPDMA_CTR1_SDW_LOG2_POS   DMA_CTR1_SDW_LOG2_Pos
#define STM32_GPDMA_CTR1_SDW_LOG2_MASK  (3U << STM32_GPDMA_CTR1_SDW_LOG2_POS)
#define STM32_GPDMA_CTR1_SDW_LOG2(n)    ((n) << STM32_GPDMA_CTR1_SDW_LOG2_POS)
#define STM32_GPDMA_CTR1_SDW_BYTE       STM32_GPDMA_CTR1_SDW_LOG2(0U)
#define STM32_GPDMA_CTR1_SDW_HALF       STM32_GPDMA_CTR1_SDW_LOG2(1U)
#define STM32_GPDMA_CTR1_SDW_WORD       STM32_GPDMA_CTR1_SDW_LOG2(2U)

#define STM32_GPDMA_CTR2_TCEM_POS       DMA_CTR2_TCEM_Pos
#define STM32_GPDMA_CTR2_TCEM_MASK      (3U << STM32_GPDMA_CTR2_TCEM_POS)
#define STM32_GPDMA_CTR2_TCEM(n)        ((n) << STM32_GPDMA_CTR2_TCEM_POS)
#define STM32_GPDMA_CTR2_TRIGPOL_POS    DMA_CTR2_TRIGPOL_Pos
#define STM32_GPDMA_CTR2_TRIGPOL_MASK   (3U << STM32_GPDMA_CTR2_TRIGPOL_POS)
#define STM32_GPDMA_CTR2_TRIGPOL(n)     ((n) << STM32_GPDMA_CTR2_TRIGPOL_POS)
#define STM32_GPDMA_CTR2_TRIGPOL_NOTRIG STM32_GPDMA_CTR2_TRIGPOL(0U)
#define STM32_GPDMA_CTR2_TRIGPOL_RISING STM32_GPDMA_CTR2_TRIGPOL(1U)
#define STM32_GPDMA_CTR2_TRIGPOL_FALLING STM32_GPDMA_CTR2_TRIGPOL(2U)
#define STM32_GPDMA_CTR2_TRIGSEL_POS    DMA_CTR2_TRIGSEL_Pos
#define STM32_GPDMA_CTR2_TRIGSEL_MASK   (0x3FU << STM32_GPDMA_CTR2_TRIGSEL_POS)
#define STM32_GPDMA_CTR2_TRIGSEL(n)     ((n) << STM32_GPDMA_CTR2_TRIGSEL_POS)
#define STM32_GPDMA_CTR2_TRIGM_POS      DMA_CTR2_TRIGM_Pos
#define STM32_GPDMA_CTR2_TRIGM_MASK     (3U << STM32_GPDMA_CTR2_TRIGM_POS)
#define STM32_GPDMA_CTR2_TRIGM(n)       ((n) << STM32_GPDMA_CTR2_TRIGM_POS)
#define STM32_GPDMA_CTR2_PFREQ          DMA_CTR2_PFREQ
#define STM32_GPDMA_CTR2_BREQ           DMA_CTR2_BREQ
#define STM32_GPDMA_CTR2_DREQ           DMA_CTR2_DREQ
#define STM32_GPDMA_CTR2_SWREQ          DMA_CTR2_SWREQ
#define STM32_GPDMA_CTR2_REQSEL_POS     DMA_CTR2_REQSEL_Pos
#define STM32_GPDMA_CTR2_REQSEL_MASK    (0xFFU << STM32_GPDMA_CTR2_REQSEL_POS)
#define STM32_GPDMA_CTR2_REQSEL(n)      ((n) << STM32_GPDMA_CTR2_REQSEL_POS)

#define STM32_GPDMA_CBR1_BRDDEC         DMA_CBR1_BRDDEC
#define STM32_GPDMA_CBR1_BRSDEC         DMA_CBR1_BRSDEC
#define STM32_GPDMA_CBR1_DDEC           DMA_CBR1_DDEC
#define STM32_GPDMA_CBR1_SDEC           DMA_CBR1_SDEC
#define STM32_GPDMA_CBR1_BRC_POS        DMA_CBR1_BRC_Pos
#define STM32_GPDMA_CBR1_BRC_MASK       (0x7FFU << STM32_GPDMA_CBR1_BRC_POS)
#define STM32_GPDMA_CBR1_BRC(n)         ((n) << STM32_GPDMA_CBR1_BRC_POS)
#define STM32_GPDMA_CBR1_BNDT_POS       DMA_CBR1_BNDT_Pos
#define STM32_GPDMA_CBR1_BNDT_MASK      (0xFFFFU << STM32_GPDMA_CBR1_BNDT_POS)
#define STM32_GPDMA_CBR1_BNDT(n)        ((n) << STM32_GPDMA_CBR1_BNDT_POS)

#define STM32_GPDMA_CTR3_DAO_POS        DMA_CTR3_DAO_Pos
#define STM32_GPDMA_CTR3_DAO_MASK       (0x1FFFU << STM32_GPDMA_CTR3_DAO_POS)
#define STM32_GPDMA_CTR3_DAO(n)         ((n) << STM32_GPDMA_CTR3_DAO_POS)
#define STM32_GPDMA_CTR3_SAO_POS        DMA_CTR3_SAO_Pos
#define STM32_GPDMA_CTR3_SAO_MASK       (0x1FFFU << STM32_GPDMA_CTR3_SAO_POS)
#define STM32_GPDMA_CTR3_SAO(n)         ((n) << STM32_GPDMA_CTR3_SAO_POS)

#define STM32_GPDMA_CBR2_BRDAO_POS      DMA_CBR2_BRDAO_Pos
#define STM32_GPDMA_CBR2_BRDAO_MASK     (0xFFFFU << STM32_GPDMA_CBR2_BRDAO_POS)
#define STM32_GPDMA_CBR2_BRDAO(n)       ((n) << STM32_GPDMA_CBR2_BRDAO_POS)
#define STM32_GPDMA_CBR2_BRSAO_POS      DMA_CBR2_BRSAO_Pos
#define STM32_GPDMA_CBR2_BRSAO_MASK     (0xFFFFU << STM32_GPDMA_CBR2_BRSAO_POS)
#define STM32_GPDMA_CBR2_BRSAO(n)       ((n) << STM32_GPDMA_CBR2_BRSAO_POS)

#define STM32_GPDMA_CLLR_UT1            DMA_CLLR_UT1
#define STM32_GPDMA_CLLR_UT2            DMA_CLLR_UT2
#define STM32_GPDMA_CLLR_UB1            DMA_CLLR_UB1
#define STM32_GPDMA_CLLR_USA            DMA_CLLR_USA
#define STM32_GPDMA_CLLR_UDA            DMA_CLLR_UDA
#define STM32_GPDMA_CLLR_UT3            DMA_CLLR_UT3
#define STM32_GPDMA_CLLR_UB2            DMA_CLLR_UB2
#define STM32_GPDMA_CLLR_ULL            DMA_CLLR_ULL
#define STM32_GPDMA_CLLR_LA_POS         DMA_CLLR_LA_Pos
#define STM32_GPDMA_CLLR_LA_MASK        (0xFFFCU << STM32_GPDMA_CLLR_LA_POS)
#define STM32_GPDMA_CLLR_LA(n)          ((n) << STM32_GPDMA_CLLR_LA_POS)
/** @< */

/**
 * @brief   Mask of interrupt flags.
 */

/**
 * @brief   Maximum number of transfers in a single operation.
 */
#define STM32_GPDMA_MAX_TRANSFER        65535

/**
 * @brief   Checks if a GPDMA priority is within the valid range.
 * @param[in] prio      DMA priority
 *
 * @retval              The check result.
 * @retval false        invalid GPDMA priority.
 * @retval true         correct GPDMA priority.
 */
#define STM32_GPDMA_IS_VALID_PRIORITY(prio)                                 \
  (((prio) >= 0U) && ((prio) <= 3U))

/**
 * @brief   Checks if a GPDMA channels mask contains all valid channels.
 *
 * @param[in] m         mask of GPDMA channels
 * @retval              The check result.
 * @retval false        invalid GPDMA channels in the mask.
 * @retval true         correct GPDMA channels.
 */
#define STM32_GPDMA_ARE_VALID_CHANNELS(m)                                   \
  (((m) & ~STM32_GPDMA_MASK_ANY) == 0U)

/**
 * @brief   Returns an unique numeric identifier for a GPDMA channel.
 *
 * @param[in] dma       the GPDMA unit number, 1 or 2.
 * @param[in] ch        the channel number
 * @return              An unique numeric channel identifier.
 */
#define STM32_GPDMA_CHANNEL_ID(dma, ch)                                     \
  ((((dma) - 1U) * STM32_GPDMA1_NUM_CHANNELS) + (ch))

/**
 * @brief   Returns a GPDMA channel identifier mask.
 *
 *
 * @param[in] dma       the GPDMA unit number, 1 or 2.
 * @param[in] ch        the channel number
 * @return              A channel mask.
 */
#define STM32_GPDMA_CHANNEL_ID_MSK(dma, ch)                                 \
  (1U << STM32_GPDMA_CHANNEL_ID(dma, ch))

/**
 * @name    DMA channels identifiers
 * @{
 */
/**
 * @brief   Returns a pointer to a @p stm32_gpdma_channel_t structure.
 *
 * @param[in] id        the channel numeric identifier
 * @return              A pointer to the @p stm32_gpdma_channel_t constant
 *                      structure associated to the GPDMA channel.
 */
#define STM32_GPDMA_CHANNEL(id)     (&__stm32_gpdma_channels[id])

#define STM32_GPDMA1_CHANNEL0       STM32_GPDMA_CHANNEL(0)
#define STM32_GPDMA1_CHANNEL1       STM32_GPDMA_CHANNEL(1)
#define STM32_GPDMA1_CHANNEL2       STM32_GPDMA_CHANNEL(2)
#define STM32_GPDMA1_CHANNEL3       STM32_GPDMA_CHANNEL(3)
#define STM32_GPDMA1_CHANNEL4       STM32_GPDMA_CHANNEL(4)
#define STM32_GPDMA1_CHANNEL5       STM32_GPDMA_CHANNEL(5)
#define STM32_GPDMA1_CHANNEL6       STM32_GPDMA_CHANNEL(6)
#define STM32_GPDMA1_CHANNEL7       STM32_GPDMA_CHANNEL(7)
#define STM32_GPDMA2_CHANNEL0       STM32_GPDMA_CHANNEL(8)
#define STM32_GPDMA2_CHANNEL1       STM32_GPDMA_CHANNEL(9)
#define STM32_GPDMA2_CHANNEL2       STM32_GPDMA_CHANNEL(10)
#define STM32_GPDMA2_CHANNEL3       STM32_GPDMA_CHANNEL(11)
#define STM32_GPDMA2_CHANNEL4       STM32_GPDMA_CHANNEL(12)
#define STM32_GPDMA2_CHANNEL5       STM32_GPDMA_CHANNEL(13)
#define STM32_GPDMA2_CHANNEL6       STM32_GPDMA_CHANNEL(14)
#define STM32_GPDMA2_CHANNEL7       STM32_GPDMA_CHANNEL(15)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32_GPDMA1_NUM_CHANNELS)
#error "STM32_GPDMA1_NUM_CHANNELS not defined in registry"
#endif

#if !defined(STM32_GPDMA2_NUM_CHANNELS)
#error "STM32_GPDMA2_NUM_CHANNELS not defined in registry"
#endif

#if (STM32_GPDMA1_NUM_CHANNELS < 0) || (STM32_GPDMA1_NUM_CHANNELS > 8)
#error "unsupported channels configuration"
#endif

#if (STM32_GPDMA2_NUM_CHANNELS < 0) || (STM32_GPDMA2_NUM_CHANNELS > 8)
#error "unsupported channels configuration"
#endif

/**
 * @brief   Total number of channels among all GPDMAs.
 */
#define STM32_GPDMA_NUM_CHANNELS                                            \
  (STM32_GPDMA1_NUM_CHANNELS + STM32_GPDMA2_NUM_CHANNELS)

#if STM32_GPDMA1_NUM_CHANNELS > 0
#if !defined(STM32_GPDMA1_MASK_FIFO2)
#error "STM32_GPDMA1_MASK_FIFO2 not defined in registry"
#endif
#if !defined(STM32_GPDMA1_MASK_FIFO4)
#error "STM32_GPDMA1_MASK_FIFO4 not defined in registry"
#endif
#if !defined(STM32_GPDMA1_MASK_FIFO4_2D)
#error "STM32_GPDMA1_MASK_FIFO4_2D not defined in registry"
#endif
#endif

#if STM32_GPDMA2_NUM_CHANNELS > 0
#if !defined(STM32_GPDMA2_MASK_FIFO2)
#error "STM32_GPDMA2_MASK_FIFO2 not defined in registry"
#endif
#if !defined(STM32_GPDMA2_MASK_FIFO4)
#error "STM32_GPDMA2_MASK_FIFO4 not defined in registry"
#endif
#if !defined(STM32_GPDMA2_MASK_FIFO4_2D)
#error "STM32_GPDMA2_MASK_FIFO4_2D not defined in registry"
#endif
#endif

/**
 * @brief   Any FIFO2 channel on any GPDMA.
 */
#define STM32_GPDMA_MASK_FIFO2                                              \
  (STM32_GPDMA1_MASK_FIFO2 | STM32_GPDMA2_MASK_FIFO2)

/**
 * @brief   Any FIFO4 channel on any GPDMA.
 */
#define STM32_GPDMA_MASK_FIFO4                                              \
  (STM32_GPDMA1_MASK_FIFO4 | STM32_GPDMA2_MASK_FIFO4)

/**
 * @brief   Any FIFO4_2D channel on any GPDMA.
 */
#define STM32_GPDMAX_MASK_FIFO4_2D                                          \
  (STM32_GPDMA1_MASK_FIFO4_2D | STM32_GPDMA2_MASK_FIFO4_2D)

/**
 * @brief   Any non-2D channel on GPDMA1.
 */
#define STM32_GPDMA1_MASK_FIFOX                                             \
  (STM32_GPDMA1_MASK_FIFO2 | STM32_GPDMA1_MASK_FIFO4)

/**
 * @brief   Any non-2D channel on GPDMA2.
 */
#define STM32_GPDMA2_MASK_FIFOX                                             \
  (STM32_GPDMA2_MASK_FIFO2 | STM32_GPDMA2_MASK_FIFO4)

/**
 * @brief   Any non-2D channel on any GPDMA.
 */
#define STM32_GPDMA_MASK_FIFOX                                              \
  (STM32_GPDMA1_MASK_FIFOX | STM32_GPDMA2_MASK_FIFOX)

/**
 * @brief   Any channel on GPDMA1.
 */
#define STM32_GPDMA1_MASK_ANY                                               \
  (STM32_GPDMA1_MASK_FIFO2 | STM32_GPDMA1_MASK_FIFO4 | STM32_GPDMA1_MASK_FIFO4_2D)

/**
 * @brief   Any channel on GPDMA2.
 */
#define STM32_GPDMA2_MASK_ANY                                               \
  (STM32_GPDMA2_MASK_FIFO2 | STM32_GPDMA2_MASK_FIFO4 | STM32_GPDMA2_MASK_FIFO4_2D)

/**
 * @brief   Any channel on any GPDMA.
 */
#define STM32_GPDMA_MASK_ANY                                                \
  (STM32_GPDMA1_MASK_ANY | STM32_GPDMA2_MASK_ANY)

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a GPDMA callback.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] csr       content of the CxSR register
 */
typedef void (*stm32_gpdmaisr_t)(void *p, uint32_t csr);

/**
 * @brief   STM32 GPDMA channel descriptor structure.
 */
typedef struct {
  DMA_Channel_TypeDef   *channel;       /**< @brief Associated channel.     */
  uint8_t               vector;         /**< @brief Associated IRQ vector.  */
} stm32_gpdma_channel_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern const stm32_gpdma_channel_t __stm32_gpdma_channels[STM32_GPDMA_NUM_CHANNELS];
#endif

/* The linker needs to export this symbol if the link mode of GPDMA is required,
   the symbol needs to be aligned to a 64k boundary and marks the base of an
   area where all symbols with names starting with __gpdma_ are collected.
   On devices with data cache this area shall be placed at beginning of a
   non-cachable area.*/
extern uint32_t __gpdma_base__;

#ifdef __cplusplus
extern "C" {
#endif
  void gpdmaInit(void);
  const stm32_gpdma_channel_t *gpdmaChannelAllocI(uint32_t cmask,
                                                  uint32_t irqprio,
                                                  stm32_gpdmaisr_t func,
                                                  void *param);
  const stm32_gpdma_channel_t *gpdmaChannelAlloc(uint32_t cmask,
                                                 uint32_t irqprio,
                                                 stm32_gpdmaisr_t func,
                                                 void *param);
  void gpdmaChannelFreeI(const stm32_gpdma_channel_t *dmachp);
  void gpdmaChannelFree(const stm32_gpdma_channel_t *dmachp);
  size_t gpdmaChannelDisable(const stm32_gpdma_channel_t *dmachp);
  void gpdmaServeInterrupt(const stm32_gpdma_channel_t *dmachp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Inline functions.                                                         */
/*===========================================================================*/

/**
 * @brief   Prepares a GPDMA channel for transfer.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a @p stm32_gpdma_channel_t structure
 * @param[in] cr        CCR register initialization value
 * @param[in] tr1       CTR1 register initialization value
 * @param[in] tr2       CTR2 register initialization value
 * @param[in] br1       CBR1 register initialization value
 * @param[in] sar       CSAR register initialization value
 * @param[in] dar       CDAR register initialization value
 * @param[in] llr       CLLR register initialization value
 *
 * @api
 */
__STATIC_FORCEINLINE
void gpdmaChannelSetupTransfer(const stm32_gpdma_channel_t *dmachp,
                               uint32_t cr, uint32_t tr1,
                               uint32_t tr2, uint32_t br1,
                               volatile const void *sar, volatile void *dar,
                               uint32_t llr) {
  DMA_Channel_TypeDef *chp = dmachp->channel;

  chp->CCR  = cr;
  chp->CTR1 = tr1;
  chp->CTR2 = tr2;
  chp->CBR1 = br1;
  chp->CSAR = (uint32_t)sar;
  chp->CDAR = (uint32_t)dar;
  chp->CLLR = llr;
}

/**
 * @brief   Prepares a GPDMA channel for a 2D transfer.
 * @note    The channel must have 2D capability.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a @p stm32_gpdma_channel_t structure
 * @param[in] cr        CCR register initialization value
 * @param[in] tr1       CTR1 register initialization value
 * @param[in] tr2       CTR2 register initialization value
 * @param[in] br1       CBR1 register initialization value
 * @param[in] sar       CSAR register initialization value
 * @param[in] dar       CDAR register initialization value
 * @param[in] tr3       CTR3 register initialization value
 * @param[in] br2       CBR2 register initialization value
 * @param[in] llr       CLLR register initialization value
 *
 * @api
 */
__STATIC_FORCEINLINE
void gpdmaChannelSetupTransfer2D(const stm32_gpdma_channel_t *dmachp,
                                 uint32_t cr, uint32_t tr1,
                                 uint32_t tr2, uint32_t br1,
                                 volatile const void *sar, volatile void *dar,
                                 uint32_t tr3, uint32_t br2, uint32_t llr) {
  DMA_Channel_TypeDef *chp = dmachp->channel;

  chp->CCR  = cr;
  chp->CTR1 = tr1;
  chp->CTR2 = tr2;
  chp->CBR1 = br1;
  chp->CSAR = (uint32_t)sar;
  chp->CDAR = (uint32_t)dar;
  chp->CTR3 = tr3;
  chp->CBR2 = br2;
  chp->CLLR = llr;
}

/**
 * @brief   Channel enable.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a @p stm32_gpdma_channel_t structure
 *
 * @api
 */
__STATIC_FORCEINLINE
void gpdmaChannelEnable(const stm32_gpdma_channel_t *dmachp) {

  dmachp->channel->CCR |= STM32_GPDMA_CCR_EN;
}

/**
 * @brief   GPDMA channel reset.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a @p stm32_gpdma_channel_t structure
 *
 * @special
 */
__STATIC_FORCEINLINE
void gpdmaChannelReset(const stm32_gpdma_channel_t *dmachp) {

  dmachp->channel->CCR |= STM32_GPDMA_CCR_RESET;
}

/**
 * @brief   GPDMA channel suspend.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a @p stm32_gpdma_channel_t structure
 *
 * @special
 */
__STATIC_FORCEINLINE
void gpdmaChannelSuspend(const stm32_gpdma_channel_t *dmachp) {

  dmachp->channel->CCR |= STM32_GPDMA_CCR_SUSP;
}

/**
 * @brief   GPDMA channel wait for idle state.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a @p stm32_gpdma_channel_t structure
 *
 * @special
 */
__STATIC_FORCEINLINE
void gpdmaChannelWaitIdle(const stm32_gpdma_channel_t *dmachp) {

  while ((dmachp->channel->CSR & STM32_GPDMA_CSR_IDLEF) == 0U) {
    /* Wait completion.*/
  }
}

/**
 * @brief   Set channel source pointer.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a @p stm32_gpdma_channel_t structure
 * @param[in] s         source pointer
 *
 * @api
 */
__STATIC_FORCEINLINE
void gpdmaChannelSetSource(const stm32_gpdma_channel_t *dmachp,
                           volatile const void *s) {

  dmachp->channel->CSAR = (uint32_t)s;
}

/**
 * @brief   Set channel destination pointer.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a @p stm32_gpdma_channel_t structure
 * @param[in] d         destination pointer
 *
 * @api
 */
__STATIC_FORCEINLINE
void gpdmaChannelSetDestination(const stm32_gpdma_channel_t *dmachp,
                                volatile const void *d) {

  dmachp->channel->CDAR = (uint32_t)d;
}

/**
 * @brief   Set channel transfer modes and triggers.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a @p stm32_gpdma_channel_t structure
 * @param[in] cr        CCR register initialization value
 * @param[in] tr1       CTR1 register initialization value
 * @param[in] tr2       CTR2 register initialization value
 * @param[in] llr       CLLR register initialization value
 *
 * @api
 */
__STATIC_FORCEINLINE
void gpdmaChannelSetMode(const stm32_gpdma_channel_t *dmachp,
                         uint32_t cr, uint32_t tr1,
                         uint32_t tr2, uint32_t llr) {
  DMA_Channel_TypeDef *chp = dmachp->channel;

  chp->CCR  = cr;
  chp->CTR1 = tr1;
  chp->CTR2 = tr2;
  chp->CLLR = llr;
}

/**
 * @brief   Set channel counters.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a @p stm32_gpdma_channel_t structure
 * @param[in] br1       transaction size
 *
 * @api
 */
__STATIC_FORCEINLINE
void gpdmaChannelSetTransactionSize(const stm32_gpdma_channel_t *dmachp,
                                    size_t br1) {

  dmachp->channel->CBR1 = (uint32_t)br1;
}

/**
 * @brief   Get channel counters.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a @p stm32_gpdma_channel_t structure
 * @return              Value of the @p BR1.BNDT field.
 *
 * @api
 */
__STATIC_FORCEINLINE
size_t gpdmaChannelGetTransactionSize(const stm32_gpdma_channel_t *dmachp) {

  return (size_t)(dmachp->channel->CBR1 & STM32_GPDMA_CBR1_BNDT_MASK);
}

#endif /* STM32_GPDMA_H */

/** @} */
