/*
 ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.
 2012,2013 Martin Schüßler and Florian Sehl, Embedded Software Laboratory,
 RWTH Aachen University

 This file is part of ChibiOS/RT.

 ChibiOS/RT is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 ChibiOS/RT is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.

 ---

 A special exception to the GPL can be applied should you wish to distribute
 a combined work that includes ChibiOS/RT, without being obliged to provide
 the source code for any proprietary components. See the file exception.txt
 for full details of how and when the exception can be applied.
 */

/**
 * @file    AT91SAM7/can_lld.c
 * @brief   AT91SAM7 CAN Driver subsystem low level driver source.
 *
 * @pre     - Make sure that the Mailbox you are receiving from is holding your
 * 			  data.
 * 			- If you have more than one use the rxfull_event provided by the
 * 			  Driver.
 * 			- In order to use the Events APIs the CH_USE_EVENTS option must
 * 			  be enabled in chconf.h.
 * 			- In order to use the CAN driver the HAL_USE_CAN option must be
 * 			  enabled in halconf.h.
 * 			- Mailbox0 is used as a Transmitmailbox.
 * 			- Mailboxes 1-7 are used as receive Mailboxes.
 *
 * @addtogroup CAN
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_CAN || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief CAN driver identifier.
 */
#if AT91SAM7_CAN_USE_CAN || defined(__DOXYGEN__)
CANDriver CAND;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   CAN[0] interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(CANIrqHandler) {

  CH_IRQ_PROLOGUE();
  can_lld_serve_interrupt(&CAND);
  AT91C_BASE_AIC->AIC_EOICR = 0;
  CH_IRQ_EPILOGUE();
}


#if defined(__GNUC__)
__attribute__((noinline))
#endif
/**
 * @brief   Handles CAN interrupts.
 *
 * @param[in] pointer to the driver that received the interrupt
 */


void can_lld_serve_interrupt(CANDriver *canp) {

  canstatus_t status;
  chSysLockFromIsr();

  status = canp->base->CAN_SR & canp->base->CAN_IMR;

  /* if AT91C_CAN_WAKEUP is set, the can test was successful
   * such that the CAN is now able to Transmit and Receive Data.
   */
  if ((status & AT91C_CAN_WAKEUP) == AT91C_CAN_WAKEUP) {
	  canp->base->CAN_IDR = AT91C_CAN_WAKEUP;
      canp->testok = AT91C_TEST_OK;
  }

  else {
	  /* check the mailboxes (MB0-MB7) and broadcast the corresponding event.*/

	    /* configure send mailbox, mailbox 0 */
	#if CAN_USE_MB0
	if(status & AT91C_CAN_MB0 && canp->mb[CAN_TxMB0-1]->CAN_MB_MSR & AT91C_CAN_MRDY) {
	 canp->base->CAN_IDR = AT91C_CAN_MB0;
	 chSemSignalI(&(canp->txsem));

	}
	#else
	#warning You need to acivate Mailbox0 to transmit.
	#endif

	#if CAN_USE_MB1
    if (status & AT91C_CAN_MB1 &&
    		canp->mb[CAN_RxMB1-1]->CAN_MB_MSR & AT91C_CAN_MRDY) {
      chSemSignalI(&(canp->rxsem));
      chEvtBroadcastFlagsI(&(canp->rxfull_event), EVENT_MASK (CAN_RxMB1-1));
      canp->base->CAN_IDR = AT91C_CAN_MB1;
    }
	#endif

	#if CAN_USE_MB2
    if (status & AT91C_CAN_MB2 &&
    		canp->mb[CAN_RxMB2-1]->CAN_MB_MSR & AT91C_CAN_MRDY) {
      chSemSignalI(&(canp->rxsem));
      chEvtBroadcastFlagsI(&(canp->rxfull_event), EVENT_MASK (CAN_RxMB2-1));
      canp->base->CAN_IDR = AT91C_CAN_MB2;
    }
	#endif

	#if CAN_USE_MB3
    if (status & AT91C_CAN_MB3 &&
    		canp->mb[CAN_RxMB3-1]->CAN_MB_MSR & AT91C_CAN_MRDY) {
      chSemSignalI(&(canp->rxsem));
      chEvtBroadcastFlagsI(&(canp->rxfull_event), EVENT_MASK (CAN_RxMB3-1));
      canp->base->CAN_IDR = AT91C_CAN_MB3;
    }
	#endif

    #if CAN_USE_MB4
    if (status & AT91C_CAN_MB4 && c
    		anp->mb[CAN_RxMB4-1]->CAN_MB_MSR & AT91C_CAN_MRDY) {
      chSemSignalI(&(canp->rxsem));
      chEvtBroadcastFlagsI(&(canp->rxfull_event), EVENT_MASK (CAN_RxMB4-1));
      canp->base->CAN_IDR = AT91C_CAN_MB4;
    }
	#endif

	#if CAN_USE_MB5
    if (status & AT91C_CAN_MB5 &&
    		canp->mb[CAN_RxMB5-1]->CAN_MB_MSR & AT91C_CAN_MRDY) {
      chSemSignalI(&(canp->rxsem));
      chEvtBroadcastFlagsI(&(canp->rxfull_event), EVENT_MASK (CAN_RxMB5-1));
      canp->base->CAN_IDR = AT91C_CAN_MB5;
    }
	#endif

	#if CAN_USE_MB6
    if (status & AT91C_CAN_MB6 &&
    		canp->mb[CAN_RxMB6-1]->CAN_MB_MSR & AT91C_CAN_MRDY) {
      chSemSignalI(&(canp->rxsem));
      chEvtBroadcastFlagsI(&(canp->rxfull_event, EVENT_MASK (CAN_RxMB6-1));
      canp->base->CAN_IDR = AT91C_CAN_MB6;
    }
	#endif

	#if CAN_USE_MB7
    if (status & AT91C_CAN_MB7 &&
    		canp->mb[CAN_RxMB7-1]->CAN_MB_MSR & AT91C_CAN_MRDY) {
      chSemSignalI(&(canp->rxsem));
      chEvtBroadcastFlagsI(&(canp->rxfull_event), EVENT_MASK (CAN_RxMB7-1));
      canp->base->CAN_IDR = AT91C_CAN_MB7;
    }
	#endif
  }

  /*check if error event is detected*/
  if ((status & 0xFFCF0000) != 0) {
	  /*The content of the SR register is copied unchanged in the upper
       half word of the listener flags mask*/
	chEvtBroadcastFlagsI(&(canp->error_event), (eventmask_t)(status&0xFFFF0000));
  }

  chSysUnlockFromIsr();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level CAN driver initialization.
 *
 * @notapi
 */
void can_lld_init(void) {
	/* Driver initialization.*/
	canObjectInit(&CAND);

	/*
	* mailbox vector initialization
	*/
#if CAN_USE_MB0
	CAND.mb[CAN_TxMB0-1] = AT91C_BASE_CAN_MB0;
#endif
#if CAN_USE_MB1
	CAND.mb[CAN_RxMB1-1] = AT91C_BASE_CAN_MB1;
#endif
#if CAN_USE_MB2
	CAND.mb[CAN_RxMB2-1] = AT91C_BASE_CAN_MB2;
#endif
#if CAN_USE_MB3
	CAND.mb[CAN_RxMB3-1] = AT91C_BASE_CAN_MB3;
#endif
#if CAN_USE_MB4
	CAND.mb[CAN_RxMB4-1] = AT91C_BASE_CAN_MB4;
#endif
#if CAN_USE_MB5
	CAND.mb[CAN_RxMB5-1] = AT91C_BASE_CAN_MB5;
#endif
#if CAN_USE_MB6
	CAND.mb[CAN_RxMB6-1] = AT91C_BASE_CAN_MB6;
#endif
#if CAN_USE_MB7
	CAND.mb[CAN_RxMB7-1] = AT91C_BASE_CAN_MB7;
#endif

	/*
	* PIO
	*/
	/* Disable interrupts on the pin(s)*/
	AT91C_BASE_PIOA->PIO_IDR = AT91C_PA19_CANRX;
	AT91C_BASE_PIOA->PIO_IDR = AT91C_PA20_CANTX;
	/* Select peripheral function A*/
	AT91C_BASE_PIOA->PIO_ASR = AT91C_PA19_CANRX;
	AT91C_BASE_PIOA->PIO_ASR = AT91C_PA20_CANTX;
	/* Disables the PIO from controlling the corresponding pin
	 * (enables peripheral control of the pin)
	 */
	AT91C_BASE_PIOA->PIO_PDR = AT91C_PA19_CANRX;
	AT91C_BASE_PIOA->PIO_PDR = AT91C_PA20_CANTX;
	/* Disable pull up */
	AT91C_BASE_PIOA->PIO_PPUDR = AT91C_PA19_CANRX | AT91C_PA20_CANTX;

	/* Configure the AIC for CAN interrupts */
	AIC_ConfigureIT(AT91C_ID_CAN, AT91C_AIC_PRIOR_HIGHEST, CANIrqHandler);
	CAND.base = AT91C_BASE_CAN;
}

/**
 * @brief   configures and starts the CAN peripheral.
 */
static void can_startsequence(CANDriver *canp) {
	  canp->state = CAN_STARTING;
	  canp->testok = AT91C_TEST_NOK;

	  /* clock */
	  /* Enable clock for PIOA */
	  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);
	  /* Enable the CAN controller peripheral clock */
	  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_CAN);

	  /* interrupts */

	  /* disable all interrupts */
	  canp->base->CAN_IDR = 0x1FFFFFFF;
	  /* Enable the interrupt on the interrupt controller*/
	  AIC_EnableIT(AT91C_ID_CAN);

	  canp->base->CAN_BR = canp->config->br;


	  /* configure send mailbox, mailbox 0 */
	#if CAN_USE_MB0
		canp->mb[CAN_TxMB0-1]->CAN_MB_MCR = 0x0;
		canp->mb[CAN_TxMB0-1]->CAN_MB_MMR = AT91C_CAN_MOT_TX | AT91C_CAN_PRIOR;
		canp->mb[CAN_TxMB0-1]->CAN_MB_MAM = 0x00000000;
		canp->mb[CAN_TxMB0-1]->CAN_MB_MID = AT91C_CAN_MIDE;
		canp->mb[CAN_TxMB0-1]->CAN_MB_MDL = 0x11223344;
		canp->mb[CAN_TxMB0-1]->CAN_MB_MDH = 0x01234567;
		canp->mb[CAN_TxMB0-1]->CAN_MB_MCR = (AT91C_CAN_MDLC & (0x8 << 16));
		canp->mb[CAN_TxMB0-1]->CAN_MB_MMR = AT91C_CAN_MOT_TX | AT91C_CAN_PRIOR;
	#else
	#warning You need to acivate Mailbox0 to transmit.
	#endif

	#if CAN_USE_MB1
		canp->mb[CAN_RxMB1-1]->CAN_MB_MCR = 0x0;
		canp->mb[CAN_RxMB1-1]->CAN_MB_MMR = AT91C_CAN_MOT_RX | AT91C_CAN_PRIOR;
		canp->mb[CAN_RxMB1-1]->CAN_MB_MAM = AT91C_CAN_MIDE
				|(AT91C_CAN_MIDvB & canp->config->mb1_acceptance_mask)
				| (AT91C_CAN_MIDvA & canp->config->mb1_acceptance_mask);
		canp->mb[CAN_RxMB1-1]->CAN_MB_MID = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb1_can_id)
				| (AT91C_CAN_MIDvA & canp->config->mb1_can_id);
		canp->mb[CAN_RxMB1-1]->CAN_MB_MDL = 0x00000000;
		canp->mb[CAN_RxMB1-1]->CAN_MB_MDH = 0x00000000;

	#endif
	#if CAN_USE_MB2
		canp->mb[CAN_RxMB2-1]->CAN_MB_MCR = 0x0;
		canp->mb[CAN_RxMB2-1]->CAN_MB_MMR = AT91C_CAN_MOT_RX | AT91C_CAN_PRIOR;
		canp->mb[CAN_RxMB2-1]->CAN_MB_MAM = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb2_acceptance_mask)
				| (AT91C_CAN_MIDvA & canp->config->mb2_acceptance_mask);
		canp->mb[CAN_RxMB2-1]->CAN_MB_MID = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb2_can_id)
				| (AT91C_CAN_MIDvA & canp->config->mb2_can_id);
			canp->mb[CAN_RxMB2-1]->CAN_MB_MDL = 0x00000000;
			canp->mb[CAN_RxMB2-1]->CAN_MB_MDH = 0x00000000;

	#endif
	#if CAN_USE_MB3
		canp->mb[CAN_RxMB3-1]->CAN_MB_MCR = 0x0;
		canp->mb[CAN_RxMB3-1]->CAN_MB_MMR = AT91C_CAN_MOT_RX | AT91C_CAN_PRIOR;
		canp->mb[CAN_RxMB3-1]->CAN_MB_MAM = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb3_acceptance_mask)
				| (AT91C_CAN_MIDvA & canp->config->mb3_acceptance_mask);
		canp->mb[CAN_RxMB3-1]->CAN_MB_MID = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb3_can_id)
				| (AT91C_CAN_MIDvA & canp->config->mb3_can_id);
		canp->mb[CAN_RxMB3-1]->CAN_MB_MDL = 0x00000000;
		canp->mb[CAN_RxMB3-1]->CAN_MB_MDH = 0x00000000;

	#endif
	#if CAN_USE_MB4
		canp->mb[CAN_RxMB4-1]->CAN_MB_MCR = 0x0;
		canp->mb[CAN_RxMB4-1]->CAN_MB_MMR = AT91C_CAN_MOT_RX | AT91C_CAN_PRIOR;_MAM = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb4_acceptance_mask)
				| (AT91C_CAN_MIDvA & canp->config->mb4_acceptance_mask);
		canp->mb[CAN_RxMB4-1]->CAN_MB_MID = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb4_can_id)
				| (AT91C_CAN_MIDvA & canp->config->mb4_can_id);
		canp->mb[CAN_RxMB4-1]->CAN_MB_MDL = 0x00000000;
		canp->mb[CAN_RxMB4-1]->CAN_MB_MDH = 0x00000000;

	#endif
	#if CAN_USE_MB5
		canp->mb[CAN_RxMB5-1]->CAN_MB_MCR = 0x0;
		canp->mb[CAN_RxMB4-1]->CAN_MB_MMR = AT91C_CAN_MOT_RX | AT91C_CAN_PRIOR;
		canp->mb[CAN_RxMB5-1]->CAN_MB_MAM = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb5_acceptance_mask)
				| (AT91C_CAN_MIDvA & canp->config->mb5_acceptance_mask);
		canp->mb[CAN_RxMB5-1]->CAN_MB_MID = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb5_can_id)
				| (AT91C_CAN_MIDvA & canp->config->mb5_can_id);
		canp->mb[CAN_RxMB5-1]->CAN_MB_MDL = 0x00000000;
		canp->mb[CAN_RxMB5-1]->CAN_MB_MDH = 0x00000000;
	#endif
	#if CAN_USE_MB6
		canp->mb[CAN_RxMB6-1]->CAN_MB_MCR = 0x0;
		canp->mb[CAN_RxMB6-1]->CAN_MB_MMR = AT91C_CAN_MOT_RX | AT91C_CAN_PRIOR;
		canp->mb[CAN_RxMB6-1]->CAN_MB_MAM = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb6_acceptance_mask)
				| (AT91C_CAN_MIDvA & canp->config->mb6_acceptance_mask);
		canp->mb[CAN_RxMB6-1]->CAN_MB_MID = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb6_can_id)
				| (AT91C_CAN_MIDvA & canp->config->mb6_can_id);
		canp->mb[CAN_RxMB6-1]->CAN_MB_MDL = 0x00000000;
		canp->mb[CAN_RxMB6-1]->CAN_MB_MDH = 0x00000000;
	#endif
	#if CAN_USE_MB7
		canp->mb[CAN_RxMB7-1]->CAN_MB_MCR = 0x0;
		canp->mb[CAN_RxMB7-1]->CAN_MB_MMR = AT91C_CAN_MOT_RX | AT91C_CAN_PRIOR;
		canp->mb[CAN_RxMB7-1]->CAN_MB_MAM = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb7_acceptance_mask)
				| (AT91C_CAN_MIDvA & canp->config->mb7_acceptance_mask);
		canp->mb[CAN_RxMB7-1]->CAN_MB_MID = AT91C_CAN_MIDE
				| (AT91C_CAN_MIDvB & canp->config->mb7_can_id)
				| (AT91C_CAN_MIDvA & canp->config->mb7_can_id);
		canp->mb[CAN_RxMB7-1]->CAN_MB_MDL = 0x00000000;
		canp->mb[CAN_RxMB7-1]->CAN_MB_MDH = 0x00000000;
	#endif
	    /* Enable the interrupt with all error cases */
		canp->base->CAN_IER = AT91C_CAN_CERR	/* (CAN) CRC Error				*/
			| AT91C_CAN_SERR 					/* (CAN) Stuffing Error			*/
			| AT91C_CAN_BERR 					/* (CAN) Bit Error				*/
			| AT91C_CAN_FERR					/* (CAN) Form Error				*/
			| AT91C_CAN_AERR;					/* (CAN) Acknowledgment Error	*/

	  /* Enable CAN and */
	  canp->base->CAN_IER = AT91C_CAN_WAKEUP;

	  /* CAN Controller Enable */
	  canp->base->CAN_MR = AT91C_CAN_CANEN;
}

/**
 * @brief   Configures and activates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */

void can_lld_start(CANDriver *canp) {

	/*start CAN*/
	can_startsequence(canp);
	/* wait until wakeup. If no wakeup after 1 sec restart. */
	chThdSleepS(1);
	while( (canp->testok) == AT91C_TEST_NOK) {
	  /* stop CAN */
	    /* Disable all interrupts */
	    canp->base->CAN_IDR = 0x1FFFFFFF;
	    /* Disable the interrupt on the interrupt controller */
	    AIC_DisableIT(AT91C_ID_CAN);
	    /* Disable the CAN controller peripheral clock */
	    AT91C_BASE_PMC->PMC_PCER = (0 << AT91C_ID_CAN);
	  /* restart CAN */
	    can_startsequence(canp);

	    chThdSleepS(1);
	}

  /*enable Mailboxes */
#if CAN_USE_MB1
	canp->base->CAN_IER = (0x1 << 1);
#endif
#if CAN_USE_MB2
	canp->base->CAN_IER = (0x1 << 2);
#endif
#if CAN_USE_MB3
    canp->base->CAN_IER = (0x1 << 3);
#endif
#if CAN_USE_MB4
	canp->base->CAN_IER = (0x1 << 4);
#endif
#if CAN_USE_MB5
	canp->base->CAN_IER = (0x1 << 5);
#endif
#if CAN_USE_MB6
	canp->base->CAN_IER = (0x1 << 6);
#endif
#if CAN_USE_MB7
	canp->base->CAN_IER = (0x1 << 7);
#endif


}

/**
 * @brief   Deactivates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_stop(CANDriver *canp) {

  /* If in ready state then disables the CAN peripheral.*/
  if (canp->state == CAN_READY) {

    /* Disable all interrupts */
    canp->base->CAN_IDR = 0x1FFFFFFF;

    /* Disable the interrupt on the interrupt controller */
    AIC_DisableIT(AT91C_ID_CAN);

    /* Disable the CAN controller peripheral clock */
    AT91C_BASE_PMC->PMC_PCER = (0 << AT91C_ID_CAN);
  }
}

/**
 * @brief   Determines whether a frame can be transmitted.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 * @return              The queue space availability.
 * @retval FALSE        no space in the transmit queue.
 * @retval TRUE         transmit slot available.
 *
 * @notapi
 */
bool_t can_lld_is_tx_empty(CANDriver *canp, canmbx_t mailbox) {

    return ((canp->mb[CAN_TxMB0-1]->CAN_MB_MSR & AT91C_CAN_MRDY) != 0);
}

/**
 * @brief  Send specified frame.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 * @param[in] ctfp      pointer to the CAN frame to be transmitted
 *
 * @notapi
 */
void can_lld_transmit(CANDriver *canp, canmbx_t mailbox, const CANTxFrame *ctfp) {
  while (!( canp->mb[CAN_TxMB0-1]->CAN_MB_MSR & AT91C_CAN_MRDY)){
  }
#if CAN_IDE_EXT
   canp->mb[CAN_TxMB0-1]->CAN_MB_MID =
		   AT91C_CAN_MIDE | (AT91C_CAN_MIDvB & ctfp->EID)
      | (AT91C_CAN_MIDvA & (ctfp->EID)); //configure the id
#else
   canp->mb[CAN_TxMB0-1]->CAN_MB_MID = AT91C_CAN_MIDE |
		   (AT91C_CAN_MIDvB & 0x0)
      | (AT91C_CAN_MIDvA & ctfp->SID); //configure the id

#endif

   /* set length of data */
   canp->mb[CAN_TxMB0-1]->CAN_MB_MCR = (AT91C_CAN_MDLC & (ctfp->DLC << 16));

   /* fill low register if 0<Length<=8 */
   if(ctfp->DLC>0)
	   canp->mb[CAN_TxMB0-1]->CAN_MB_MDL = ctfp->data32[0];
   /* fill high register 4<Length<=8 */
   if(ctfp->DLC>4)
	   canp->mb[CAN_TxMB0-1]->CAN_MB_MDH = ctfp->data32[1];
   canp->base->CAN_IER = 1 << CAN_TxMB0-1;
   canp->base->CAN_TCR = 1 << 0; //send msg
}

/**
 * @brief   check if some data is available mailbox.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox	mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 * @return              The queue space availability.
 * @retval FALSE		mailbox is empty
 * @retval TRUE			some data in the mailbox
 *
 * @notapi
 */
bool_t can_lld_is_rx_nonempty(CANDriver *canp, canmbx_t mailbox) {

  if(mailbox == CAN_ANY_MAILBOX){
	/* get status of all receive mailboxes (Mailbox0 is transmit mailbox) */
	  if((canp->base->CAN_SR & 0xFE) != 0){
	  	  return TRUE;
      }
  } else{
	/* get status of specified mailbox */
	if((canp->base->CAN_SR & (1<<(mailbox-1))) != 0){
		/* data available */
		return TRUE;
	} else{
		/* no data */
		return FALSE;
	}
  }
}

/**
 * @brief   Receives a frame from the specified mailbox.
 *
 * @pre     Make sure that the Mailbox you are receiving from is holding
 * 			your data. If you have more than one use the rxfull_event
 * 			provided by the Driver.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 * @param[out] crfp     pointer to the buffer where the CAN frame is copied
 *
 * @notapi
 */
void can_lld_receive(CANDriver *canp, canmbx_t mailbox, CANRxFrame *crfp) {
  if(mailbox != CAN_ANY_MAILBOX){
	  /* get length of data */
	  crfp->DLC = (canp->mb[mailbox-1]->CAN_MB_MSR &
			  AT91C_CAN_MDLC) >> 16;
	  /* store data */
	  if(crfp->DLC>0){
		  crfp->data32[0] = canp->mb[mailbox-1]->CAN_MB_MDL;
	  }
	  if(crfp->DLC>=4){
		  crfp->data32[1] = canp->mb[mailbox-1]->CAN_MB_MDH;
	  }
  }
  else{
	  /* TODO implement can_lld_receive for CAN_ANY_MAILBOX */
  }
  /* clear register */
  canp->base->CAN_TCR = (0x1 << (mailbox-1));
  /* reenable interrupt */
  canp->base->CAN_IER = (0x1 << (mailbox-1));
}

#if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
/**
 * @brief   Enters the sleep mode.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_sleep(CANDriver *canp) {
	/* Wait till Tx Mailbox is ready */
	while(!((canp->mb[CAN_TxMB0-1]->CAN_MB_MSR & AT91C_CAN_MRDY) == AT91C_CAN_MRDY)){
	}
	/* enable low power mode */
	canp->base->CAN_MR |= AT91C_CAN_LPM;
	/* wait till CAN is in low power mode */
	while(!((canp->base->CAN_SR & AT91C_CAN_SLEEP) == AT91C_CAN_SLEEP)){
	}
	/* disable Clock */
	AT91C_BASE_PMC->PMC_PCDR = (1 << AT91C_ID_CAN);

}

/**
 * @brief   Enforces leaving the sleep mode.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_wakeup(CANDriver *canp) {
	/* enable clock */
	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_CAN);
	/* disable low power mode */
	canp->base->CAN_MR &= ~(AT91C_CAN_SLEEP);
	while(!((canp->base->CAN_SR & AT91C_CAN_WAKEUP)==AT91C_CAN_WAKEUP)){
	}

}
#endif /* CAN_USE_SLEEP_MODE */

#endif /* HAL_USE_CAN */

/** @} */
