/*
    ChibiOS - Copyright (C) 2006..2019 Giovanni Di Sirio

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
 * @file    MDMAv1/stm32_mdma.h
 * @brief   MDMA helper driver header.
 *
 * @addtogroup STM32_MDMA
 * @{
 */

#ifndef STM32_MDMA_H
#define STM32_MDMA_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Total number of MDMA streams.
 */
#define STM32_MDMA_CHANNELS         16U

/**
 * @brief   Mask of the ISR bits passed to the MDMA callback functions.
 */
#define STM32_MDMA_ISR_MASK         0x1FU

/**
 * @brief   Checks if a MDMA priority is within the valid range.
 * @param[in] prio      MDMA priority
 *
 * @retval              The check result.
 * @retval false        invalid MDMA priority.
 * @retval true         correct MDMA priority.
 */
#define STM32_MDMA_IS_VALID_PRIORITY(prio) (((prio) >= 0U) && ((prio) <= 3U))

/**
 * @brief   Special stream identifier
 */
#define STM32_MDMA_CHANNEL_ID_ANY   STM32_MDMA_CHANNELS

/**
 * @name    MDMA streams identifiers
 * @{
 */
/**
 * @brief   Returns a pointer to a stm32_mdma_channel_t structure.
 *
 * @param[in] id        the stream numeric identifier
 * @return              A pointer to the stm32_mdma_channel_t constant structure
 *                      associated to the MDMA stream.
 */
#define STM32_MDMA_CHANNEL(id)       (&__stm32_mdma_channels[id])

#define STM32_MDMA_CHANNEL0         STM32_MDMA_CHANNEL(0)
#define STM32_MDMA_CHANNEL1         STM32_MDMA_CHANNEL(1)
#define STM32_MDMA_CHANNEL2         STM32_MDMA_CHANNEL(2)
#define STM32_MDMA_CHANNEL3         STM32_MDMA_CHANNEL(3)
#define STM32_MDMA_CHANNEL4         STM32_MDMA_CHANNEL(4)
#define STM32_MDMA_CHANNEL5         STM32_MDMA_CHANNEL(5)
#define STM32_MDMA_CHANNEL6         STM32_MDMA_CHANNEL(6)
#define STM32_MDMA_CHANNEL7         STM32_MDMA_CHANNEL(7)
#define STM32_MDMA_CHANNEL8         STM32_MDMA_CHANNEL(8)
#define STM32_MDMA_CHANNEL9         STM32_MDMA_CHANNEL(9)
#define STM32_MDMA_CHANNEL10        STM32_MDMA_CHANNEL(10)
#define STM32_MDMA_CHANNEL11        STM32_MDMA_CHANNEL(11)
#define STM32_MDMA_CHANNEL12        STM32_MDMA_CHANNEL(12)
#define STM32_MDMA_CHANNEL13        STM32_MDMA_CHANNEL(13)
#define STM32_MDMA_CHANNEL14        STM32_MDMA_CHANNEL(14)
#define STM32_MDMA_CHANNEL15        STM32_MDMA_CHANNEL(15)
/** @} */

/**
 * @name    CISR register constants
 * @{
 */
#define STM32_MDMA_CISR_TEIF        (1U << 0)
#define STM32_MDMA_CISR_CTCIF       (1U << 1)
#define STM32_MDMA_CISR_BRTIF       (1U << 2)
#define STM32_MDMA_CISR_BTIF        (1U << 3)
#define STM32_MDMA_CISR_TCIF        (1U << 4)
#define STM32_MDMA_CISR_CRQA        (1U << 16)
/** @} */

/**
 * @name    CIFCR register constants
 * @{
 */
#define STM32_MDMA_CIFCR_CTEIF      (1U << 0)
#define STM32_MDMA_CIFCR_CCTCIF     (1U << 1)
#define STM32_MDMA_CIFCR_CBRTIF     (1U << 2)
#define STM32_MDMA_CIFCR_CBTIF      (1U << 3)
#define STM32_MDMA_CIFCR_CTCIF      (1U << 4)
/** @} */

/**
 * @name    CESR register constants
 * @{
 */
#define STM32_MDMA_CESR_TEA_MASK    (127U << 0)
#define STM32_MDMA_CESR_TED         (1U << 7)
#define STM32_MDMA_CESR_TELD        (1U << 8)
#define STM32_MDMA_CESR_TEMD        (1U << 9)
#define STM32_MDMA_CESR_ASE         (1U << 10)
#define STM32_MDMA_CESR_BSE         (1U << 11)
/** @} */

/**
 * @name    CCR register constants
 * @{
 */
#define STM32_MDMA_CCR_RESET_VALUE  0x00000000U
#define STM32_MDMA_CCR_EN           (1U << 0)
#define STM32_MDMA_CCR_TEIE         (1U << 1)
#define STM32_MDMA_CCR_CTCIE        (1U << 2)
#define STM32_MDMA_CCR_BRTIE        (1U << 3)
#define STM32_MDMA_CCR_BTIE         (1U << 4)
#define STM32_MDMA_CCR_TCIE         (1U << 5)
#define STM32_MDMA_CCR_PL_MASK      (3U << 6)
#define STM32_MDMA_CCR_PL(n)        ((n) << 6)
#define STM32_MDMA_CCR_BEX          (1U << 12)
#define STM32_MDMA_CCR_HEX          (1U << 13)
#define STM32_MDMA_CCR_WEX          (1U << 14)
#define STM32_MDMA_CCR_SWRQ         (1U << 16)
/** @} */

/**
 * @name    CTCR register constants
 * @{
 */
#define STM32_MDMA_CTCR_RESET_VALUE     0x00000000U

#define STM32_MDMA_CTCR_SINC_MASK       (3U << 0)
#define STM32_MDMA_CTCR_SINC(n)         ((n) << 0)
#define STM32_MDMA_CTCR_SINC_FIXED      STM32_MDMA_CTCR_SINC(0U)
#define STM32_MDMA_CTCR_SINC_INC        STM32_MDMA_CTCR_SINC(1U)
#define STM32_MDMA_CTCR_SINC_DEC        STM32_MDMA_CTCR_SINC(3U)

#define STM32_MDMA_CTCR_DINC_MASK       (3U << 2)
#define STM32_MDMA_CTCR_DINC(n)         ((n) << 2)
#define STM32_MDMA_CTCR_DINC_FIXED      STM32_MDMA_CTCR_DINC(0U)
#define STM32_MDMA_CTCR_DINC_INC        STM32_MDMA_CTCR_DINC(1U)
#define STM32_MDMA_CTCR_DINC_DEC        STM32_MDMA_CTCR_DINC(3U)

#define STM32_MDMA_CTCR_SSIZE_MASK      (3U << 4)
#define STM32_MDMA_CTCR_SSIZE(n)        ((n) << 4)
#define STM32_MDMA_CTCR_SSIZE_BYTE      STM32_MDMA_CTCR_SSIZE(0U)
#define STM32_MDMA_CTCR_SSIZE_HALF      STM32_MDMA_CTCR_SSIZE(1U)
#define STM32_MDMA_CTCR_SSIZE_WORD      STM32_MDMA_CTCR_SSIZE(2U)
#define STM32_MDMA_CTCR_SSIZE_DWORD     STM32_MDMA_CTCR_SSIZE(3U)

#define STM32_MDMA_CTCR_DSIZE_MASK      (3U << 6)
#define STM32_MDMA_CTCR_DSIZE(n)        ((n) << 6)
#define STM32_MDMA_CTCR_DSIZE_BYTE      STM32_MDMA_CTCR_DSIZE(0U)
#define STM32_MDMA_CTCR_DSIZE_HALF      STM32_MDMA_CTCR_DSIZE(1U)
#define STM32_MDMA_CTCR_DSIZE_WORD      STM32_MDMA_CTCR_DSIZE(2U)
#define STM32_MDMA_CTCR_DSIZE_DWORD     STM32_MDMA_CTCR_DSIZE(3U)

#define STM32_MDMA_CTCR_SINCOS_MASK     (3U << 8)
#define STM32_MDMA_CTCR_SINCOS(n)       ((n) << 8)
#define STM32_MDMA_CTCR_SINCOS_BYTE     STM32_MDMA_CTCR_SINCOS(0U)
#define STM32_MDMA_CTCR_SINCOS_HALF     STM32_MDMA_CTCR_SINCOS(1U)
#define STM32_MDMA_CTCR_SINCOS_WORD     STM32_MDMA_CTCR_SINCOS(2U)
#define STM32_MDMA_CTCR_SINCOS_DWORD    STM32_MDMA_CTCR_SINCOS(3U)

#define STM32_MDMA_CTCR_DINCOS_MASK     (3U << 10)
#define STM32_MDMA_CTCR_DINCOS(n)       ((n) << 10)
#define STM32_MDMA_CTCR_DINCOS_BYTE     STM32_MDMA_CTCR_DINCOS(0U)
#define STM32_MDMA_CTCR_DINCOS_HALF     STM32_MDMA_CTCR_DINCOS(1U)
#define STM32_MDMA_CTCR_DINCOS_WORD     STM32_MDMA_CTCR_DINCOS(2U)
#define STM32_MDMA_CTCR_DINCOS_DWORD    STM32_MDMA_CTCR_DINCOS(3U)

#define STM32_MDMA_CTCR_SBURST_MASK     (7U << 12)
#define STM32_MDMA_CTCR_SBURST(n)       ((n) << 12)
#define STM32_MDMA_CTCR_SBURST_1        STM32_MDMA_CTCR_SBURST(0U)
#define STM32_MDMA_CTCR_SBURST_2        STM32_MDMA_CTCR_SBURST(1U)
#define STM32_MDMA_CTCR_SBURST_4        STM32_MDMA_CTCR_SBURST(2U)
#define STM32_MDMA_CTCR_SBURST_8        STM32_MDMA_CTCR_SBURST(3U)
#define STM32_MDMA_CTCR_SBURST_16       STM32_MDMA_CTCR_SBURST(4U)
#define STM32_MDMA_CTCR_SBURST_32       STM32_MDMA_CTCR_SBURST(5U)
#define STM32_MDMA_CTCR_SBURST_64       STM32_MDMA_CTCR_SBURST(6U)
#define STM32_MDMA_CTCR_SBURST_128      STM32_MDMA_CTCR_SBURST(7U)

#define STM32_MDMA_CTCR_DBURST_MASK     (7U << 15)
#define STM32_MDMA_CTCR_DBURST(n)       ((n) << 15)
#define STM32_MDMA_CTCR_DBURST_1        STM32_MDMA_CTCR_DBURST(0U)
#define STM32_MDMA_CTCR_DBURST_2        STM32_MDMA_CTCR_DBURST(1U)
#define STM32_MDMA_CTCR_DBURST_4        STM32_MDMA_CTCR_DBURST(2U)
#define STM32_MDMA_CTCR_DBURST_8        STM32_MDMA_CTCR_DBURST(3U)
#define STM32_MDMA_CTCR_DBURST_16       STM32_MDMA_CTCR_DBURST(4U)
#define STM32_MDMA_CTCR_DBURST_32       STM32_MDMA_CTCR_DBURST(5U)
#define STM32_MDMA_CTCR_DBURST_64       STM32_MDMA_CTCR_DBURST(6U)
#define STM32_MDMA_CTCR_DBURST_128      STM32_MDMA_CTCR_DBURST(7U)

#define STM32_MDMA_CTCR_TLEN_MASK       (127U << 18)
#define STM32_MDMA_CTCR_TLEN(n)         ((n) << 18)

#define STM32_MDMA_CTCR_PKE             (1U << 25)

#define STM32_MDMA_CTCR_PAM_MASK        (3U << 26)
#define STM32_MDMA_CTCR_PAM(n)          ((n) << 26)
#define STM32_MDMA_CTCR_PAM_RIGHT       STM32_MDMA_CTCR_PAM(0U)
#define STM32_MDMA_CTCR_PAM_RIGHT_SE    STM32_MDMA_CTCR_PAM(1U)
#define STM32_MDMA_CTCR_PAM_LEFT        STM32_MDMA_CTCR_PAM(2U)

#define STM32_MDMA_CTCR_TRGM_MASK       (3U << 28)
#define STM32_MDMA_CTCR_TRGM(n)         ((n) << 28)
#define STM32_MDMA_CTCR_TRGM_BUFFER     STM32_MDMA_CTCR_TRGM(0U)
#define STM32_MDMA_CTCR_TRGM_BLOCK      STM32_MDMA_CTCR_TRGM(1U)
#define STM32_MDMA_CTCR_TRGM_REP_BLOCK  STM32_MDMA_CTCR_TRGM(2U)
#define STM32_MDMA_CTCR_TRGM_WHOLE      STM32_MDMA_CTCR_TRGM(3U)

#define STM32_MDMA_CTCR_SWRM            (1U << 30)

#define STM32_MDMA_CTCR_BWM             (1U << 31)
/** @} */

/**
 * @name    BNDTR register constants
 * @{
 */
#define STM32_MDMA_CBNDTR_BNDT_MASK     (0x1FFFFU << 0)
#define STM32_MDMA_CBNDTR_BNDT(n)       ((n) << 0)
#define STM32_MDMA_CBNDTR_BRSUM         (1U << 18)
#define STM32_MDMA_CBNDTR_BRDUM         (1U << 19)
#define STM32_MDMA_CBNDTR_BRC_MASK      (0xFFFU << 20)
#define STM32_MDMA_CBNDTR_BRC(n)        ((n) << 20)
/** @} */

/**
 * @name    CBRUR register constants
 * @{
 */
#define STM32_MDMA_CBRUR_SUV_MASK       (0xFFFFU << 0)
#define STM32_MDMA_CBRUR_SUV(n)         ((n) << 0)
#define STM32_MDMA_CBRUR_DUV_MASK       (0xFFFFU << 16)
#define STM32_MDMA_CBRUR_DUV(n)         ((n) << 16)
/** @} */

/**
 * @name    CTBR register constants
 * @{
 */
#define STM32_MDMA_CTBR_TSEL_MASK       (0x3FU << 0)
#define STM32_MDMA_CTBR_TSEL(n)         ((n) << 0)
#define STM32_MDMA_CTBR_TSEL_SBUS       (1U << 16)
#define STM32_MDMA_CTBR_TSEL_DBUS       (1U << 17)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32_HAS_MDMA)
#error "STM32_HAS_MDMA missing in registry"
#endif

#if !defined(STM32_HAS_DMA2)
#error "STM32_HAS_DMA2 missing in registry"
#endif

#if !defined(STM32_MDMA_CH0_HANDLER)
#error "STM32_MDMA_CH0_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH1_HANDLER)
#error "STM32_MDMA_CH1_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH2_HANDLER)
#error "STM32_MDMA_CH2_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH3_HANDLER)
#error "STM32_MDMA_CH3_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH4_HANDLER)
#error "STM32_MDMA_CH4_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH5_HANDLER)
#error "STM32_MDMA_CH5_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH6_HANDLER)
#error "STM32_MDMA_CH6_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH7_HANDLER)
#error "STM32_MDMA_CH7_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH8_HANDLER)
#error "STM32_MDMA_CH8_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH9_HANDLER)
#error "STM32_MDMA_CH9_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH10_HANDLER)
#error "STM32_MDMA_CH10_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH11_HANDLER)
#error "STM32_MDMA_CH11_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH12_HANDLER)
#error "STM32_MDMA_CH12_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH13_HANDLER)
#error "STM32_MDMA_CH13_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH14_HANDLER)
#error "STM32_MDMA_CH14_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH15_HANDLER)
#error "STM32_MDMA_CH15_HANDLER missing in registry"
#endif

#if !defined(STM32_MDMA_CH0_NUMBER)
#error "STM32_MDMA_CH0_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH1_NUMBER)
#error "STM32_MDMA_CH1_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH2_NUMBER)
#error "STM32_MDMA_CH2_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH3_NUMBER)
#error "STM32_MDMA_CH3_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH4_NUMBER)
#error "STM32_MDMA_CH4_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH5_NUMBER)
#error "STM32_MDMA_CH5_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH6_NUMBER)
#error "STM32_MDMA_CH6_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH7_NUMBER)
#error "STM32_MDMA_CH7_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH8_NUMBER)
#error "STM32_MDMA_CH8_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH9_NUMBER)
#error "STM32_MDMA_CH9_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH10_NUMBER)
#error "STM32_MDMA_CH10_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH11_NUMBER)
#error "STM32_MDMA_CH11_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH12_NUMBER)
#error "STM32_MDMA_CH12_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH13_NUMBER)
#error "STM32_MDMA_CH13_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH14_NUMBER)
#error "STM32_MDMA_CH14_NUMBER missing in registry"
#endif

#if !defined(STM32_MDMA_CH15_NUMBER)
#error "STM32_MDMA_CH15_NUMBER missing in registry"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   STM32 MDMA ISR function type.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] flags     content of the CISR register
 */
typedef void (*stm32_mdmaisr_t)(void *p, uint32_t flags);

/**
 * @brief   STM32 MDMA stream descriptor structure.
 */
typedef struct {
  MDMA_Channel_TypeDef  *channel;       /**< @brief Associated MDMA channel. */
  uint32_t              selfindex;      /**< @brief Index to self in array. */
  uint32_t              vector;         /**< @brief Associated IRQ vector.  */
} stm32_mdma_channel_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Associates a source address to a MDMA stream.
 * @pre     The stream must have been allocated using @p mdmaChannelAlloc().
 * @post    After use the stream can be released using @p mdmaChannelFree().
 *
 * @param[in] mdmachp   pointer to a stm32_mdma_channel_t structure
 * @param[in] addr      value to be written in the CSAR register
 *
 * @xclass
 */
#define mdmaChannelSetSourceX(mdmachp, addr) do {                           \
  (mdmachp)->channel->CSAR  = (uint32_t)(addr);                             \
} while (0)

/**
 * @brief   Associates a memory destination to a MDMA stream.
 * @pre     The stream must have been allocated using @p mdmaChannelAlloc().
 * @post    After use the stream can be released using @p mdmaChannelFree().
 *
 * @param[in] mdmachp   pointer to a stm32_mdma_channel_t structure
 * @param[in] addr      value to be written in the CDAR register
 *
 * @xclass
 */
#define mdmaChannelSetDestinationX(mdmachp, addr) do {                      \
  (mdmachp)->channel->CDAR  = (uint32_t)(addr);                             \
} while (0)

/**
 * @brief   Sets parameters related to the transaction size.
 * @pre     The stream must have been allocated using @p mdmaChannelAlloc().
 * @post    After use the stream can be released using @p mdmaChannelFree().
 *
 * @param[in] mdmachp   pointer to a stm32_mdma_channel_t structure
 * @param[in] size      number of bytes per block
 * @param[in] n         number of blocks repetitions
 * @param[in] opt       other option bits for the CBNDTR register
 *
 * @xclass
 */
#define mdmaChannelSetTransactionSizeX(mdmachp, size, n, opt) do {          \
  (mdmachp)->channel->CBNDTR  = (uint32_t)STM32_MDMA_CBNDTR_BNDT(size) |    \
                                (uint32_t)STM32_MDMA_CBNDTR_BRC(n) |        \
                                (uint32_t)opt;                              \
} while (0)

/**
 * @brief   Programs the stream mode settings.
 * @pre     The stream must have been allocated using @p mdmaChannelAlloc().
 * @post    After use the stream can be released using @p mdmaChannelFree().
 *
 * @param[in] mdmachp   pointer to a stm32_mdma_channel_t structure
 * @param[in] ctcr      value to be written in the CTCR register
 * @param[in] ccr       value to be written in the CCR register
 *
 * @xclass
 */
#define mdmaChannelSetModeX(mdmachp, ctcr, ccr) do {                        \
    (mdmachp)->channel->CTCR = (uint32_t)(ctcr);                            \
    (mdmachp)->channel->CCR  = (uint32_t)(ccr);                             \
} while (0)

/**
 * @brief   MDMA stream enable.
 * @pre     The stream must have been allocated using @p mdmaChannelAlloc().
 * @post    After use the stream can be released using @p mdmaChannelFree().
 *
 * @param[in] mdmachp   pointer to a stm32_mdma_channel_t structure
 *
 * @xclass
 */
#define mdmaChannelEnableX(mdmachp) do {                                    \
  (mdmachp)->channel->CCR |= STM32_MDMA_CCR_EN;                             \
} while (0)

/**
 * @brief   MDMA stream interrupt sources clear.
 * @pre     The stream must have been allocated using @p mdmaChannelAlloc().
 * @post    After use the stream can be released using @p mdmaChannelFree().
 *
 * @param[in] mdmachp   pointer to a stm32_mdma_channel_t structure
 *
 * @xclass
 */
#define mdmaStreamClearInterruptX(mdmachp) do {                             \
  *(mdmachp)->CIFCR = (STM32_MDMA_CIFCR_CTEIF  |                            \
                       STM32_MDMA_CIFCR_CBRTIF |                            \
                       STM32_MDMA_CIFCR_CBRTIF |                            \
                       STM32_MDMA_CIFCR_CCTCIF |                            \
                       STM32_MDMA_CIFCR_CTEIF)                              \
} while (0)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern const stm32_mdma_channel_t __stm32_mdma_channels[STM32_MDMA_CHANNELS];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void mdmaInit(void);
  const stm32_mdma_channel_t *mdmaChannelAllocI(uint32_t id,
                                                uint32_t priority,
                                                stm32_mdmaisr_t func,
                                                void *param);
  const stm32_mdma_channel_t *mdmaChannelAlloc(uint32_t id,
                                               uint32_t priority,
                                               stm32_mdmaisr_t func,
                                               void *param);
  void mdmaChannelFreeI(const stm32_mdma_channel_t *mdmachp);
  void mdmaChannelFree(const stm32_mdma_channel_t *mdmachp);
  void mdmaStreamDisableX(const stm32_mdma_channel_t *mdmachp);
#ifdef __cplusplus
}
#endif

#endif /* STM32_MDMA_H */

/** @} */
