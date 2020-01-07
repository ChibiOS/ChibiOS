/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    hal_can_lld.h
 * @brief   STM32 CAN subsystem low level driver header.
 *
 * @addtogroup CAN
 * @{
 */

#ifndef HAL_CAN_LLD_H
#define HAL_CAN_LLD_H

#if (HAL_USE_CAN == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Maximum number of bytes in data of CAN packets.
 */
#define CAN_MAX_DLC_BYTES           64

/**
 * @brief   Number of transmit mailboxes.
 */
#define CAN_TX_MAILBOXES            1

/**
 * @brief   Number of receive mailboxes.
 */
#define CAN_RX_MAILBOXES            2

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    STM32 configuration options
 * @{
 */
/**
 * @brief   CAN1 driver enable switch.
 * @details If set to @p TRUE the support for CAN1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_CAN_USE_CAN1) || defined(__DOXYGEN__)
#define STM32_CAN_USE_CAN1                  FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an CAN driver.
 */
typedef struct CANDriver CANDriver;

/**
 * @brief   Type of a transmission mailbox index.
 */
typedef uint32_t canmbx_t;

#if defined(CAN_ENFORCE_USE_CALLBACKS) || defined(__DOXYGEN__)
/**
 * @brief   Type of a CAN notification callback.
 *
 * @param[in] canp      pointer to the @p CANDriver object triggering the
 *                      callback
 * @param[in] flags     flags associated to the mailbox callback
 */
typedef void (*can_callback_t)(CANDriver *canp, uint32_t flags);
#endif

/**
 * @brief   CAN transmission frame.
 * @note    Accessing the frame data as word16 or word32 is not portable because
 *          machine data endianness, it can be still useful for a quick filling.
 */
typedef struct {
  /*lint -save -e46 [6.1] Standard types are fine too.*/
  union {
    
    struct {
      union {
        uint32_t            EID:29;         /**< @brief Extended Identifier */
        struct {
          uint32_t          _R1:18;         /**< @brief Reserved for Offset.*/
          uint16_t          SID:11;         /**< @brief Standard Identifier.*/
          uint8_t           RTR:1;          /**< @brief Remote Transmit Req.*/
          uint8_t           XTD:1;          /**< @brief Extended Identifier.*/
          uint8_t           ESI:1;          /**< @brief Error State Indicat */
        };
      };
      uint16_t              _R2:16;         /**< @brief Reserved.           */
      uint8_t               DLC:4;          /**< @brief Data Length Code.   */
      uint8_t               BPS:1;          /**< @brief Accepted non-match. */
      uint8_t               FDF:1;          /**< @brief FDCAN Format.       */
      uint8_t               _R3:1;          /**< @brief .         */
      uint8_t               EFC:1;          /**< @brief Event FIFO Control. */
      uint8_t               MM:8;           /**< @brief Message Event Marker*/
    };
    uint32_t                header32[2];
  };

  /*lint -restore*/
  union {
    uint8_t                 data8[CAN_MAX_DLC_BYTES]; /**< @brief Frame data.     */
    uint16_t                data16[CAN_MAX_DLC_BYTES / 2];
    uint32_t                data32[CAN_MAX_DLC_BYTES / 4];
  };
} CANTxFrame;

/**
 * @brief   CAN received frame.
 * @note    Accessing the frame data as data16 or data32 is not portable because
 *          machine data endianness, it can be still useful for a quick filling.
 */
typedef struct {
  /*lint -save -e46 [6.1] Standard types are fine too.*/
  union {
    struct {
      union {
        uint32_t            EID:29;         /**< @brief Standard identifier.*/
        struct {
          uint32_t          _R1:18;         /**< @brief Standard identifier.*/
          uint16_t          SID:11;         /**< @brief Standard identifier.*/
          uint8_t           RTR:1;          /**< @brief Remote Transmit Req.*/
          uint8_t           XTD:1;          /**< @brief Extended Identifier.*/
          uint8_t           ESI:1;          /**< @brief .        */
        };
      };
      uint16_t              RXTS:16;        /**< @brief .         */
      uint8_t               DLC:4;          /**< @brief Data Length Code.   */
      uint8_t               BRS:1;          /**< @brief .         */
      uint8_t               FDF:1;          /**< @brief FDCAN Format.       */
      uint8_t               _R2:1;          /**< @brief .         */
      uint8_t               FIDX:7;         /**< @brief .         */
      uint8_t               ANMF:1;         /**< @brief .         */
    };
    uint32_t                header32[2];
  };
      /*lint -restore*/
  union {
    uint8_t                 data8[CAN_MAX_DLC_BYTES];   /**< @brief Frame data.   */
    uint16_t                data16[CAN_MAX_DLC_BYTES / 2];
    uint32_t                data32[CAN_MAX_DLC_BYTES / 4];
  };
} CANRxFrame;

/**
 * @brief   CAN standard filter.
 * @note    Accessing the frame data as data16 or data32 is not portable because
 *          machine data endianness, it can be still useful for a quick filling.
 */
typedef struct {
  union {
    struct {
      uint16_t              SFID2:11;
      uint8_t               _R1:5;
      uint16_t              SFID1:11;
      uint8_t               SFEC:3;
      uint8_t               SFT:2;
    };
    union {
      uint32_t              data32;
      uint16_t              data16[2];
      uint8_t               data8[4];
    };
  };
} CANRxStandardFilter;


/**
 * @brief   CAN extended filter.
 * @note    Accessing the frame data as data16 or data32 is not portable because
 *          machine data endianness, it can be still useful for a quick filling.
 */
typedef struct {
  union {
    struct {
      uint32_t              EFID1:29;
      uint8_t               EFEC:3;
      uint32_t              EFID2:29;
      uint8_t               _R1:1;
      uint8_t               EFT:2;
    };
    union {
      uint32_t              data32[2];
      uint16_t              data16[4];
      uint8_t               data8[8];
    };
  };
} CANRxExtendedFilter;

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  uint8_t                   anfs;     /* Accept non-masked standard messages */
  uint8_t                   anfe;     /* Accept non-masked extended messages */
  bool                      dar;      /* Disable automatic reply */
  bool                      loopback;
  bool                      monitor;
  bool                      brs;      /* Bit-rate switching */
  bool                      fd;       /* Flexible Datarate CAN */
} CANConfig;

void canConfigObjectInit(CANConfig * config);
/**
 * @brief   Structure representing an CAN driver.
 */
struct CANDriver {
  /**
   * @brief   Driver state.
   */
  canstate_t                state;
  /**
   * @brief   Current configuration data.
   */
  const CANConfig           *config;
  /**
   * @brief   Transmission threads queue.
   */
  threads_queue_t           txqueue;
  /**
   * @brief   Receive threads queue.
   */
  threads_queue_t           rxqueue;
#if (CAN_ENFORCE_USE_CALLBACKS == FALSE) || defined (__DOXYGEN__)
  /**
   * @brief   One or more frames become available.
   * @note    After broadcasting this event it will not be broadcasted again
   *          until the received frames queue has been completely emptied. It
   *          is <b>not</b> broadcasted for each received frame. It is
   *          responsibility of the application to empty the queue by
   *          repeatedly invoking @p chReceive() when listening to this event.
   *          This behavior minimizes the interrupt served by the system
   *          because CAN traffic.
   * @note    The flags associated to the listeners will indicate which
   *          receive mailboxes become non-empty.
   */
  event_source_t            rxfull_event;
  /**
   * @brief   One or more transmission mailbox become available.
   * @note    The flags associated to the listeners will indicate which
   *          transmit mailboxes become empty.
   */
  event_source_t            txempty_event;
  /**
   * @brief   A CAN bus error happened.
   * @note    The flags associated to the listeners will indicate the
   *          error(s) that have occurred.
   */
  event_source_t            error_event;
#if (CAN_USE_SLEEP_MODE == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief   Entering sleep state event.
   */
  event_source_t            sleep_event;
  /**
   * @brief   Exiting sleep state event.
   */
  event_source_t            wakeup_event;
#endif
#else /* CAN_ENFORCE_USE_CALLBACKS == TRUE */
  /**
   * @brief   One or more frames become available.
   * @note    After calling this function it will not be called again
   *          until the received frames queue has been completely emptied. It
   *          is <b>not</b> called for each received frame. It is
   *          responsibility of the application to empty the queue by
   *          repeatedly invoking @p chTryReceiveI().
   *          This behavior minimizes the interrupt served by the system
   *          because CAN traffic.
   */
  can_callback_t            rxfull_cb;
  /**
   * @brief   One or more transmission mailbox become available.
   * @note    The flags associated to the callback will indicate which
   *          transmit mailboxes become empty.
   */
  can_callback_t            txempty_cb;
  /**
   * @brief   A CAN bus error happened.
   */
  can_callback_t            error_cb;
#if (CAN_USE_SLEEP_MODE == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief   Exiting sleep state.
   */
  can_callback_t            wakeup_cb;
#endif
#endif
  FDCAN_GlobalTypeDef       *can;
  uint32_t                  *ram_base;
  /* End of the mandatory fields.*/
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (STM32_CAN_USE_CAN1 == TRUE) && !defined(__DOXYGEN__)
extern CANDriver CAND1;
#endif

#if (STM32_CAN_USE_CAN2 == TRUE) && !defined(__DOXYGEN__)
extern CANDriver CAND2;
#endif

#if (STM32_CAN_USE_CAN3 == TRUE) && !defined(__DOXYGEN__)
extern CANDriver CAND3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void can_lld_init(void);
  void can_lld_start(CANDriver *canp);
  void can_lld_stop(CANDriver *canp);
  bool can_lld_is_tx_empty(CANDriver *canp, canmbx_t mailbox);
  void can_lld_transmit(CANDriver *canp,
                        canmbx_t mailbox,
                        const CANTxFrame *ctfp);
  bool can_lld_is_rx_nonempty(CANDriver *canp, canmbx_t mailbox);
  void can_lld_receive(CANDriver *canp,
                       canmbx_t mailbox,
                       CANRxFrame *crfp);
  void can_lld_abort(CANDriver *canp,
                     canmbx_t mailbox);
#if CAN_USE_SLEEP_MODE == TRUE
  void can_lld_sleep(CANDriver *canp);
  void can_lld_wakeup(CANDriver *canp);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_CAN == TRUE */

#endif /* HAL_CAN_LLD_H */

/** @} */
