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
 * @file    AT91SAM7/can_lld.h
 * @brief   AT91SAM7 CAN Driver subsystem low level driver header.
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

#ifndef _CAN_LLD_H_
#define _CAN_LLD_H_

#if HAL_USE_CAN || defined(__DOXYGEN__)



/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   This switch defines whether the driver implementation supports
 *          a low power switch mode with automatic an wakeup feature.
 */
#define CAN_SUPPORTS_SLEEP  		TRUE

/**
 * @brief   This implementation supports one transmit mailboxes.
 */
#define CAN_TX_MAILBOXES            1

/**
 * @brief   This implementation supports two receive mailboxes.
 */
#define CAN_RX_MAILBOXES            7

/**
 * @brief   CAN mailboxes (Mailbox0 should always enabled to transmit)
 */
#ifndef CAN_USE_MB0
#define CAN_USE_MB0 TRUE
#endif
#ifndef CAN_USE_MB1
#define CAN_USE_MB1 TRUE
#endif
#ifndef CAN_USE_MB2
#define CAN_USE_MB2 TRUE
#endif
#ifndef CAN_USE_MB3
#define CAN_USE_MB3 TRUE
#endif
#ifndef CAN_USE_MB4
#define CAN_USE_MB4 FALSE
#endif
#ifndef CAN_USE_MB5
#define CAN_USE_MB5 FALSE
#endif
#ifndef CAN_USE_MB6
#define CAN_USE_MB6 FALSE
#endif
#ifndef CAN_USE_MB7
#define CAN_USE_MB7 FALSE
#endif


#define CAN_IDE_STD                 FALSE        /**< @brief Standard id.    */
#define CAN_IDE_EXT                 TRUE         /**< @brief Extended id.    */

#define CAN_RTR_DATA                FALSE        /**< @brief Data frame.     */
#define CAN_RTR_REMOTE              TRUE         /**< @brief Remote frame.   */

/**
 * @brief timeout until can_lld_start stops waiting
 * on CAN and ends up in error state
 */
#define AT91C_CAN_TIMEOUT			100


/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   CAN driver enable switch.
 * @details If set to @p TRUE the support for CAN0 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(AT91SAM7_CAN_USE_CAN) || defined(__DOXYGEN__)
#define AT91SAM7_CAN_USE_CAN                  TRUE
#endif

/**
 * @brief   CAN interrupt priority level setting.
 */
#if !defined(AT91SAM7_CAN_CAN_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define AT91SAM7_CAN_CAN_IRQ_PRIORITY         11
#endif

/**
 * @brief   Sleep mode related APIs inclusion switch.
 * @note    This switch is enforced to @p FALSE if the driver implementation
 *          does not support the sleep mode.
 */
#if CAN_SUPPORTS_SLEEP || defined(__DOXYGEN__)
#if !defined(CAN_USE_SLEEP_MODE) || defined(__DOXYGEN__)
#define CAN_USE_SLEEP_MODE  TRUE
#endif
#else /* !CAN_SUPPORTS_SLEEP */
#define CAN_USE_SLEEP_MODE  FALSE
#endif /* !CAN_SUPPORTS_SLEEP */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if CAN_USE_SLEEP_MODE && !CAN_SUPPORTS_SLEEP
#error "CAN sleep mode not supported in this architecture"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Type of a transmission mailbox index
 */
typedef uint32_t canmbx_t;

/**
 *  wakeup status possible states
 */
typedef enum {
	AT91C_TEST_NOK = 0,                    /**< waiting for wakeup event  	*/
	AT91C_TEST_OK = 1                     /**< wakeup event occured		*/
} canteststate_t;

/**
 * @brief possible receive mailboxes.
 * These Mailboxes get an offset to ensure that the Mailbox are not uninitialized.
 *
 */
typedef enum {
	CAN_TxMB0 = 1,		/**< Transmit Mailbox.          */
	CAN_RxMB1 = 2,		/**< Receive Mailbox 1.         */
	CAN_RxMB2 = 3,		/**< Receive Mailbox 2.         */
	CAN_RxMB3 = 4,		/**< Receive Mailbox 3.         */
	CAN_RxMB4 = 5,		/**< Receive Mailbox 4.         */
	CAN_RxMB5 = 6,		/**< Receive Mailbox 5.         */
	CAN_RxMB6 = 7,		/**< Receive Mailbox 6.         */
	CAN_RxMB7 = 8		/**< Receive Mailbox 7.         */
} canmailbox_t;

/**
 * @brief   CAN status flags.
 */
typedef uint32_t canstatus_t;

/**
 * @brief   CAN transmission frame.
 * @note    Accessing the frame data as word16 or word32 is not portable
 *          because machine data endianness, it can be still useful for a
 *          quick filling.
 */
typedef struct {
  struct {
    uint8_t DLC:4; /**< @brief Data length.        */
    uint8_t RTR:1; /**< @brief Frame type.         */
    uint8_t IDE:1; /**< @brief Identifier type.    */
  };
  union {
    struct {
      uint32_t SID:11; /**< @brief Standard identifier.*/
    };
    struct {
      uint32_t EID:29; /**< @brief Extended identifier.*/
    };
  };
  union {
    uint8_t data8[8]; /**< @brief Frame data.         */
    uint16_t data16[4]; /**< @brief Frame data.         */
    uint32_t data32[2]; /**< @brief Frame data.         */
  };
}CANTxFrame;

/**
 * @brief   CAN received frame.
 * @note    Accessing the frame data as word16 or word32 is not portable
 *          because machine data endianness, it can be still useful for a
 *          quick filling.
 */
typedef struct {
  struct {
    uint8_t DLC:4; /**< @brief Data length.        */
    uint8_t RTR:1; /**< @brief Frame type.         */
    uint8_t IDE:1; /**< @brief Identifier type.    */
  };
  union {
    struct {
      uint32_t SID:11; /**< @brief Standard identifier.*/
    };
    struct {
      uint32_t EID:29; /**< @brief Extended identifier.*/
    };
  };
  union {
    uint8_t data8[8]; /**< @brief Frame data.         */
    uint16_t data16[4]; /**< @brief Frame data.         */
    uint32_t data32[2]; /**< @brief Frame data.         */
  };
  canmailbox_t mbid; /**< @brief mailbox to read from  */
}CANRxFrame;

/**
 * @brief   CAN filter.
 * @note    NOT used on this platform
 */
typedef struct {
}CANFilter;

/**
 * @brief   Driver configuration structure.
 */
typedef struct {

#if CAN_USE_MB1
	uint32_t mb1_can_id; /**< @brief mailbox 1 can id  */
	uint32_t mb1_acceptance_mask; /**< @brief mailbox 1 acceptance mask  */
#endif
#if CAN_USE_MB2
	uint32_t mb2_can_id; /**< @brief mailbox 2 can id  */
	uint32_t mb2_acceptance_mask; /**< @brief mailbox 2 acceptance mask  */
#endif
#if CAN_USE_MB3
	uint32_t mb3_can_id; /**< @brief mailbox 3 can id  */
	uint32_t mb3_acceptance_mask; /**< @brief mailbox 3 acceptance mask  */
#endif
#if CAN_USE_MB4
	uint32_t mb4_can_id; /**< @brief mailbox 4 can id  */
	uint32_t mb4_acceptance_mask; /**< @brief mailbox 4 acceptance mask  */
#endif
#if CAN_USE_MB5
	uint32_t mb5_can_id; /**< @brief mailbox 5 can id  */
	uint32_t mb5_acceptance_mask; /**< @brief mailbox 5 acceptance mask  */
#endif
#if CAN_USE_MB6
	uint32_t mb6_can_id; /**< @brief mailbox 6 can id  */
	uint32_t mb6_acceptance_mask; /**< @brief mailbox 6 acceptance mask  */
#endif
#if CAN_USE_MB7
	uint32_t mb7_can_id; /**< @brief mailbox 7 can id  */
	uint32_t mb7_acceptance_mask; /**< @brief mailbox 7 acceptance mask  */
#endif
  /**
   * @brief value of the BR register
   * @note  use 0x00050301 for 1MBit/s!
   *
   */
  uint32_t br;

}CANConfig;


/**
 * @brief   Structure representing an CAN driver.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
typedef struct {
  /**
   * @brief Driver state.
   */
  canstate_t state;
  /**
   * @brief Current configuration data.
   */
  const CANConfig *config;
  /**
   * @brief Transmission queue semaphore.
   */
  Semaphore txsem;
  /**
   * @brief Receive queue semaphore.
   */
  Semaphore rxsem;
  /**
   * @brief One frame for one specified mailbox become available.
   * @note  The number of the mailbox will be broadcasted to all listening threads.
   *        It is responsibility of the application(thread) to empty the specified mailbox
   *        by invoking @p chReceive() when listening to this event.
   */
  EventSource rxfull_event;
  /**
   * @brief transmission slot become available.
   */
  EventSource txempty_event;
  /**
   * @brief A CAN bus error happened.
   */
  EventSource error_event;
#if CAN_USE_SLEEP_MODE || defined (__DOXYGEN__)
  /**
   * @brief Entering sleep state event.
   */
  EventSource sleep_event;
  /**
   * @brief Exiting sleep state event.
   */
  EventSource wakeup_event;
#endif /* CAN_USE_SLEEP_MODE */
  /* End of the mandatory fields.*/
  /**
   * @brief Driver test OK (wakeup event occurred).
   */
  volatile canteststate_t testok;
  /**
   * @brief Pointer to the Mailboxes registers block.
   */
  AT91PS_CAN_MB mb[8];
  /**
   * @brief Pointer to the CAN registers.
   */
  AT91PS_CAN base;
}CANDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/
  /**
   * @brief create event mask using enum of the corresponding mailbox.
   */
#define CAN_EVENT_MASK(mailbox) EVENT_MASK(mailbox-1)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if AT91SAM7_CAN_USE_CAN && !defined(__DOXYGEN__)
extern CANDriver CAND;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void can_lld_init(void);
  void can_lld_serve_interrupt(CANDriver *canp);
  void can_lld_start(CANDriver *canp);
  void can_lld_stop(CANDriver *canp);
  bool_t can_lld_is_tx_empty(CANDriver *canp, canmbx_t mailbox);
  void can_lld_transmit(CANDriver *canp,
  	  	  				canmbx_t mailbox,
		  	  	  	  	const CANTxFrame *crfp);
  bool_t can_lld_is_rx_nonempty(CANDriver *canp, canmbx_t mailbox);
  void can_lld_receive(CANDriver *canp,
  	  	  			   canmbx_t mailbox,
		  	  	  	   CANRxFrame *ctfp);
#if CAN_USE_SLEEP_MODE
  void can_lld_sleep(CANDriver *canp);
  void can_lld_wakeup(CANDriver *canp);

#endif /* CAN_USE_SLEEP_MODE */
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_CAN */

#endif /* _CAN_LLD_H_ */

/** @} */
