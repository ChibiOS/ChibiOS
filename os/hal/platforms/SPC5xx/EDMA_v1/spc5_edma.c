/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

  CH_IRQ_PROLOGUE();

  /* TODO: Pass to the drivers somehow.*/
  chSysHalt();

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
  if (channels[0] != NULL)
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
  if (channels[1] != NULL)
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
  if (channels[2] != NULL)
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
  if (channels[3] != NULL)
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
  if (channels[4] != NULL)
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
  if (channels[5] != NULL)
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
  if (channels[6] != NULL)
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
  if (channels[7] != NULL)
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
  if (channels[8] != NULL)
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
  if (channels[9] != NULL)
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
  if (channels[10] != NULL)
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
  if (channels[11] != NULL)
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
  if (channels[12] != NULL)
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
  if (channels[13] != NULL)
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
  if (channels[14] != NULL)
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
  if (channels[15] != NULL)
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
  if (channels[16] != NULL)
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
  if (channels[17] != NULL)
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
  if (channels[18] != NULL)
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
  if (channels[19] != NULL)
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
  if (channels[20] != NULL)
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
  if (channels[21] != NULL)
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
  if (channels[22] != NULL)
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
  if (channels[23] != NULL)
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
  if (channels[24] != NULL)
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
  if (channels[25] != NULL)
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
  if (channels[26] != NULL)
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
  if (channels[27] != NULL)
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
  if (channels[28] != NULL)
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
  if (channels[29] != NULL)
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
  if (channels[30] != NULL)
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
  if (channels[31] != NULL)
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
             (ccfg->dma_irq_prio < 16) &&
             (ccfg->dma_func != NULL) && (ccfg->dma_error_func != NULL),
             "edmaChannelAllocate");

#if SPC5_EDMA_HAS_MUX
  /* TODO: MUX handling.*/
  channel = EDMA_ERROR;
  return channel;
#else /* !SPC5_EDMA_HAS_MUX */
  channel = (edma_channel_t)ccfg->dma_periph;
  if (channels[channel] != NULL)
    return EDMA_ERROR;  /* Already taken.*/
  channels[channel] = ccfg;
  return channel;
#endif /* !SPC5_EDMA_HAS_MUX */
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

  channels[channel] = NULL;
}

/** @} */
