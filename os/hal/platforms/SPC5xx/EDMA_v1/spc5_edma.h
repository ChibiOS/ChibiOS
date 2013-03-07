/*
 * Licensed under ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file    SPC5xx/edma.h
 * @brief   EDMA helper driver header.
 *
 * @addtogroup SPC5xx_EDMA
 * @{
 */

#ifndef _EDMA_H_
#define _EDMA_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   EDMA channel allocation error.
 */
#define EDMA_ERROR                  -1

/**
 * @name    EDMA mode constants
 * @{
 */
#define EDMA_TCD_MODE_START         (1U << 0)
#define EDMA_TCD_MODE_INT_END       (1U << 1)
#define EDMA_TCD_MODE_INT_HALF      (1U << 2)
#define EDMA_TCD_MODE_DREQ          (1U << 3)
#define EDMA_TCD_MODE_ACTIVE        (1U << 6)
#define EDMA_TCD_MODE_DONE          (1U << 7)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Default EDMA CR register initialization.
 */
#if !defined(SPC5_EDMA_ERROR_HANDLER) || defined(__DOXYGEN__)
#define SPC5_EDMA_CR_SETTING                0x0000C400
#endif

/**
 * @brief   EDMA critical error handler, must not return.
 */
#if !defined(SPC5_EDMA_ERROR_HANDLER) || defined(__DOXYGEN__)
#define SPC5_EDMA_ERROR_HANDLER()           chSysHalt()
#endif


/**
 * @brief   EDMA error handler IRQ priority.
 */
#if !defined(SPC5_EDMA_ERROR_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_EDMA_ERROR_IRQ_PRIO            12
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !SPC5_HAS_EDMAA
#error "this device does not have an eDMA unit"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of and EDMA channel number.
 */
typedef int32_t edma_channel_t;

/**
 * @brief   Type of an EDMA TCD.
 */
typedef struct {
  union {
    uint32_t              word[8];
  };
} edma_tcd_t;

/**
 * @brief   DMA callback type.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 */
typedef void (*edma_callback_t)(edma_channel_t channel, void *p);

/**
 * @brief   DMA error callback type.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 * @param[in] esr       content of the ESR register
 */
typedef void (*edma_error_callback_t)(edma_channel_t channel,
                                      void *p,
                                      uint32_t esr);

/**
 * @brief   Type of an EDMA channel configuration structure.
 */
typedef struct {
  uint8_t               dma_periph;     /**< @brief Peripheral to be
                                             associated to the channel.     */
  uint8_t               dma_prio;       /**< @brief Priority register value
                                             for this channel.              */
  uint8_t               dma_irq_prio;   /**< @brief IRQ priority level for
                                             this channel.                  */
  edma_callback_t       dma_func;       /**< @brief Channel callback,
                                             can be NULL if not required.  */
  edma_error_callback_t dma_error_func; /**< @brief Channel error callback,
                                             can be NULL if not required.  */
  void                  *dma_param;     /**< @brief Channel callback param. */
} edma_channel_config_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the TCD address associated to a channel.
 *
 * @param[in] channel   the channel number
 * @return              A pointer to an @p edma_tcd_t structure.
 *
 * @api
 */
#define edmaGetTCD(channel) ((edma_tcd_t *)&EDMA.TCD[channel])

/**
 * @brief   Sets the word 0 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] src       the source address
 *
 * @api
 */
#define edmaTCDSetWord0(tcdp, src)                                          \
  ((tcdp)->word[0] = (uint32_t)(src))

/**
 * @brief   Sets the word 1 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] ssize     the source width
 * @param[in] dst       the destination width
 * @param[in] soff      the source increment value
 *
 * @api
 */
#define edmaTCDSetWord1(tcdp, ssize, dsize, soff)                           \
  ((tcdp)->word[1] = (((uint32_t)(ssize) << 24) |                           \
                      ((uint32_t)(dsize) << 16) |                           \
                      ((uint32_t)(soff) << 0)))

/**
 * @brief   Sets the word 2 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] nbytes    the inner counter value
 *
 * @api
 */
#define edmaTCDSetWord2(tcdp, nbytes)                                       \
  ((tcdp)->word[2] = (uint32_t)(nbytes))

/**
 * @brief   Sets the word 3 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] slast      the adjustment value
 *
 * @api
 */
#define edmaTCDSetWord3(tcdp, slast)                                        \
  ((tcdp)->word[3] = (uint32_t)(slast))

/**
 * @brief   Sets the word 4 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] dst       the destination address
 *
 * @api
 */
#define edmaTCDSetWord4(tcdp, dst)                                          \
  ((tcdp)->word[4] = (uint32_t)(dst))

/**
 * @brief   Sets the word 5 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] citer     the current outer counter value
 * @param[in] doff      the destination increment value
 *
 * @api
 */
#define edmaTCDSetWord5(tcdp, citer, doff)                                  \
  ((tcdp)->word[5] = (((uint32_t)(citer) << 16) |                           \
                      ((uint32_t)(doff) << 0)))

/**
 * @brief   Sets the word 6 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] dlast     the adjustment value
 *
 * @api
 */
#define edmaTCDSetWord6(tcdp, dlast)                                        \
  ((tcdp)->word[6] = (uint32_t)(dlast))

/**
 * @brief   Sets the word 7 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] biter     the base outer counter value
 * @param[in] mode      the mode value
 *
 * @api
 */
#define edmaTCDSetWord7(tcdp, biter, mode)                                  \
  ((tcdp)->word[7] = (((uint32_t)(biter) << 16) |                           \
                      ((uint32_t)(mode) << 0)))

/**
 * @brief   Starts or restarts an EDMA channel.
 *
 * @param[in] channel   the channel number
 *
 * @api
 */
#define edmaChannelStart(channel) (EDMA.SERQR.R = (channel))

/**
 * @brief   Stops an EDMA channel.
 *
 * @param[in] channel   the channel number
 *
 * @api
 */
#define edmaChannelStop(channel) {                                          \
  EDMA.CERQR.R = (channel);                                                 \
  EDMA.CDSBR.R = (channel);                                                 \
}

/**
 * @brief   EDMA channel setup.
 *
 * @param[in] channel   eDMA channel number
 * @param[in] src       source address
 * @param[in] dst       destination address
 * @param[in] soff      source address offset
 * @param[in] doff      destination address offset
 * @param[in] ssize     source transfer size
 * @param[in] dsize     destination transfer size
 * @param[in] nbytes    minor loop count
 * @param[in] iter      major loop count
 * @param[in] dlast     last destination address adjustment
 * @param[in] slast     last source address adjustment
 * @param[in] mode      LSW of TCD register 7
 *
 * @api
 */
#define edmaChannelSetup(channel, src, dst, soff, doff, ssize, dsize,       \
                         nbytes, iter, slast, dlast, mode) {                \
  edma_tcd_t *tcdp = edmaGetTCD(channel);                                   \
  edmaTCDSetWord0(tcdp, src);                                               \
  edmaTCDSetWord1(tcdp, ssize, dsize, soff);                                \
  edmaTCDSetWord2(tcdp, nbytes);                                            \
  edmaTCDSetWord3(tcdp, slast);                                             \
  edmaTCDSetWord4(tcdp, dst);                                               \
  edmaTCDSetWord5(tcdp, iter, doff);                                        \
  edmaTCDSetWord6(tcdp, dlast);                                             \
  edmaTCDSetWord7(tcdp, iter, mode);                                        \
}

#if 0
tcdp->word[0] = (uint32_t)(src);                                          \
tcdp->word[1] = ((uint32_t)(ssize) << 24) | ((uint32_t)(dsize) << 16) |   \
                 (uint32_t)(soff);                                        \
tcdp->word[2] = (uint32_t)(nbytes);                                       \
tcdp->word[3] = (uint32_t)(slast);                                        \
tcdp->word[0] = (uint32_t)(dst);                                          \
tcdp->word[5] = ((uint32_t)(iter) << 16) | (uint32_t)(doff);              \
tcdp->word[6] = (uint32_t)(dlast);                                        \
tcdp->word[7] = ((uint32_t)(iter) << 16) | (uint32_t)(mode);
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void edmaInit(void);
  edma_channel_t edmaChannelAllocate(const edma_channel_config_t *ccfg);
  void edmaChannelRelease(edma_channel_t channel);
#ifdef __cplusplus
}
#endif

#endif /* _EDMA_H_ */

/** @} */
