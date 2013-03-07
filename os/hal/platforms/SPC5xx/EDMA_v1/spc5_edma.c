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
 * @file    SPC5xx/edma.c
 * @brief   EDMA helper driver code.
 *
 * @addtogroup SPC5xx_EDMA
 * @{
 */

#include "ch.h"
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

/**
 * @brief   Configurations for the various EDMA channels.
 */
static const edma_channel_config_t *channels[SPC5_EDMA_NCHANNELS];

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   EDMA error interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector10) {
  edma_channel_t channel;
  uint32_t erl, esr = EDMA.ESR.R;

  CH_IRQ_PROLOGUE();

  /* Scanning for errors.*/
  channel = 0;
  while (((erl = EDMA.ERL.R) != 0) && (channel < SPC5_EDMA_NCHANNELS)) {

    if ((erl & (1U << channel)) != 0) {
      /* Error flag cleared.*/
      EDMA.CER.R = channel;

      /* If the channel is not associated then the error is simply discarded
         else the error callback is invoked.*/
      if ((channels[channel] != NULL) &&
          (channels[channel]->dma_error_func != NULL))
        channels[channel]->dma_error_func(channel,
                                          channels[channel]->dma_param,
                                          esr);
      channel++;
    }
  }

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 0 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector11) {

  CH_IRQ_PROLOGUE();

  if (channels[0] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 0;
  channels[0]->dma_func(0, channels[0]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 1 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector12) {

  CH_IRQ_PROLOGUE();

  if (channels[1] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 1;
  channels[1]->dma_func(1, channels[1]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 2 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector13) {

  CH_IRQ_PROLOGUE();

  if (channels[2] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 2;
  channels[2]->dma_func(2, channels[2]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 3 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector14) {

  CH_IRQ_PROLOGUE();

  if (channels[3] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 3;
  channels[3]->dma_func(3, channels[3]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 4 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector15) {

  CH_IRQ_PROLOGUE();

  if (channels[4] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 4;
  channels[4]->dma_func(4, channels[4]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 5 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector16) {

  CH_IRQ_PROLOGUE();

  if (channels[5] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 5;
  channels[5]->dma_func(5, channels[5]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 6 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector17) {

  CH_IRQ_PROLOGUE();

  if (channels[6] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 6;
  channels[6]->dma_func(6, channels[6]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 7 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector18) {

  CH_IRQ_PROLOGUE();

  if (channels[7] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 7;
  channels[7]->dma_func(7, channels[7]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 8 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector19) {

  CH_IRQ_PROLOGUE();

  if (channels[8] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 8;
  channels[8]->dma_func(8, channels[8]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 9 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector20) {

  CH_IRQ_PROLOGUE();

  if (channels[9] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 9;
  channels[9]->dma_func(9, channels[9]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 10 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector21) {

  CH_IRQ_PROLOGUE();

  if (channels[10] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 10;
  channels[10]->dma_func(10, channels[10]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 11 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector22) {

  CH_IRQ_PROLOGUE();

  if (channels[11] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 11;
  channels[11]->dma_func(11, channels[11]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 12 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector23) {

  CH_IRQ_PROLOGUE();

  if (channels[12] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 12;
  channels[12]->dma_func(12, channels[12]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 13 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector24) {

  CH_IRQ_PROLOGUE();

  if (channels[13] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 13;
  channels[13]->dma_func(13, channels[13]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 14 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector25) {

  CH_IRQ_PROLOGUE();

  if (channels[14] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 14;
  channels[14]->dma_func(14, channels[14]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 15 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector26) {

  CH_IRQ_PROLOGUE();

  if (channels[15] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 15;
  channels[15]->dma_func(15, channels[15]->dma_param);

  CH_IRQ_EPILOGUE();
}

#if (SPC5_EDMA_NCHANNELS > 16) || defined(__DOXYGEN__)
/**
 * @brief   EDMA channel 16 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector27) {

  CH_IRQ_PROLOGUE();

  if (channels[16] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 16;
  channels[16]->dma_func(16, channels[16]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 17 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector28) {

  CH_IRQ_PROLOGUE();

  if (channels[17] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 17;
  channels[17]->dma_func(17, channels[17]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 18 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector29) {

  CH_IRQ_PROLOGUE();

  if (channels[18] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 18;
  channels[18]->dma_func(18, channels[18]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 19 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector30) {

  CH_IRQ_PROLOGUE();

  if (channels[19] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 19;
  channels[19]->dma_func(19, channels[19]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 20 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector31) {

  CH_IRQ_PROLOGUE();

  if (channels[20] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 20;
  channels[20]->dma_func(20, channels[20]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 21 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector32) {

  CH_IRQ_PROLOGUE();

  if (channels[21] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 21;
  channels[21]->dma_func(21, channels[21]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 22 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector33) {

  CH_IRQ_PROLOGUE();

  if (channels[22] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 22;
  channels[22]->dma_func(22, channels[22]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 23 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector34) {

  CH_IRQ_PROLOGUE();

  if (channels[23] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 23;
  channels[23]->dma_func(23, channels[23]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 24 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector35) {

  CH_IRQ_PROLOGUE();

  if (channels[24] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 24;
  channels[24]->dma_func(24, channels[24]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 25 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector36) {

  CH_IRQ_PROLOGUE();

  if (channels[25] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 25;
  channels[25]->dma_func(25, channels[25]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 26 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector37) {

  CH_IRQ_PROLOGUE();

  if (channels[26] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 26;
  channels[26]->dma_func(26, channels[26]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 27 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector38) {

  CH_IRQ_PROLOGUE();

  if (channels[27] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 27;
  channels[27]->dma_func(27, channels[27]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 28 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector39) {

  CH_IRQ_PROLOGUE();

  if (channels[28] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 28;
  channels[28]->dma_func(28, channels[28]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 29 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector40) {

  CH_IRQ_PROLOGUE();

  if (channels[29] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 29;
  channels[29]->dma_func(29, channels[29]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 30 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector41) {

  CH_IRQ_PROLOGUE();

  if (channels[30] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 30;
  channels[30]->dma_func(30, channels[30]->dma_param);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EDMA channel 31 interrupt.
 *
 * @isr
 */
CH_IRQ_HANDLER(vector42) {

  CH_IRQ_PROLOGUE();

  if (channels[31] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  EDMA.CIRQR.R = 31;
  channels[31]->dma_func(31, channels[31]->dma_param);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_EDMA_NCHANNELS > 16 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   EDMA driver initialization.
 *
 * @special
 */
void edmaInit(void) {
  unsigned i;

  EDMA.CR.R    = SPC5_EDMA_CR_SETTING;
  EDMA.ERQRL.R = 0x00000000;
  EDMA.EEIRL.R = 0x00000000;
  EDMA.IRQRL.R = 0xFFFFFFFF;
  EDMA.ERL.R =   0xFFFFFFFF;
  for (i = 0; i < SPC5_EDMA_NCHANNELS; i++)
    EDMA.CPR[i].R = 0;

  /* Error interrupt source.*/
  INTC.PSR[10].R = SPC5_EDMA_ERROR_IRQ_PRIO;
}

/**
 * @brief   EDMA channel allocation.
 *
 * @param[in] ccfg      channel configuration
 * @return              The channel TCD pointer.
 * @retval EDMA_ERROR   if the channel cannot be allocated.
 *
 * @special
 */
edma_channel_t edmaChannelAllocate(const edma_channel_config_t *ccfg) {
  edma_channel_t channel;

  chDbgCheck((ccfg != NULL) && ((ccfg->dma_prio & 15) < 16) &&
             (ccfg->dma_irq_prio < 16),
             "edmaChannelAllocate");

#if SPC5_EDMA_HAS_MUX
  /* TODO: MUX handling.*/
  channel = EDMA_ERROR;
#else /* !SPC5_EDMA_HAS_MUX */
  channel = (edma_channel_t)ccfg->dma_periph;
  if (channels[channel] != NULL)
    return EDMA_ERROR;  /* Already taken.*/
#endif /* !SPC5_EDMA_HAS_MUX */

  /* Associating the configuration to the channel.*/
  channels[channel] = ccfg;

  /* If an error callback is defined then the error interrupt source is
     enabled for the channel.*/
  if (ccfg->dma_error_func != NULL)
    EDMA.SEEIR.R = channel;

  /* Setting up IRQ priority for the selected channel.*/
  INTC.PSR[11 + channel].R = ccfg->dma_irq_prio;

  return channel;
}

/**
 * @brief   EDMA channel allocation.
 *
 * @param[in] channel   the channel number
 *
 * @special
 */
void edmaChannelRelease(edma_channel_t channel) {

  chDbgCheck((channel < 0) && (channel >= SPC5_EDMA_NCHANNELS),
             "edmaChannelAllocate");
  chDbgAssert(channels[channel] != NULL,
              "edmaChannelRelease(), #1",
              "not allocated");

  /* Enforcing a stop.*/
  edmaChannelStop(channel);

  /* Clearing ISR sources for the channel.*/
  EDMA.CIRQR.R = channel;
  EDMA.CEEIR.R = channel;
  EDMA.CER.R   = channel;

  /* The channels is flagged as available.*/
  channels[channel] = NULL;
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
 * @param[in] dlast_sga Last Destination Address Adjustment or
 *                      Scatter Gather Address
 * @param[in] slast     last source address adjustment
 * @param[in] mode      LSW of TCD register 7
 */
void edmaChannelSetupx(edma_channel_t channel,
                      void *src,
                      void *dst,
                      uint32_t soff,
                      uint32_t doff,
                      uint32_t ssize,
                      uint32_t dsize,
                      uint32_t nbytes,
                      uint32_t iter,
                      uint32_t slast,
                      uint32_t dlast,
                      uint32_t mode) {

  edma_tcd_t *tcdp = edmaGetTCD(channel);

  tcdp->word[0] = (uint32_t)src;
  tcdp->word[1] = (ssize << 24) | (dsize << 16) | soff;
  tcdp->word[2] = nbytes;
  tcdp->word[3] = slast;
  tcdp->word[0] = (uint32_t)dst;
  tcdp->word[5] = (iter << 16) | doff;
  tcdp->word[6] = dlast;
  tcdp->word[7] = (iter << 16) | mode;
}

/** @} */
